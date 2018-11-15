/*Steve McCoy
*/

#include "CInvoke.h"
#include "Ctor.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include <functional>

CInvoke *explicit_superctor = 0, *explicit_thisctor = 0;

CInvoke::CInvoke(CInvoke::Invoker i, std::list<Expr*> *a)
  : invoker(i), args(a){
  assert(args != 0);
}

void CInvoke::dump() const{
  static const char *is[] = { " this", "super" };
  std::cerr << " Constructor invocation: " << is[invoker] << '(';
  std::list<Expr*>::const_iterator a = args->begin();
  for(; a != args->end(); ++a)
    std::cerr << (*a)->type()->str() << ", ";
  std::cerr << ")\n";
}

Node *CInvoke::analyze(){
  if(!Type::current())
    error("Can't directly invoke a constructor outside of a method");
  std::list<Expr*>::iterator a;
  for(a = args->begin(); a != args->end(); ++a)
    try{
      // test if member of enclosing class!
      *a = dynamic_cast<Expr*>((*a)->analyze());
    }catch(SyntaxError& s){ /* go on */ }

  std::list<Ctor*> candidates;
  if(invoker == This){
    Type::current()->applicable_ctors(candidates, args);
    explicit_thisctor = this;
  }else{
    Type::current()->super()->applicable_ctors(candidates,args);
    explicit_superctor = this;
  }

  if(candidates.size() == 0)
    error("No matching constructor");
  ctor = most_specific(candidates);
  if(!ctor)
    error("No most-specific constructor");
  return this;
}

Ctor *CInvoke::most_specific(std::list<Ctor*>& ctors){
  ctors.sort(std::mem_fun(&Ctor::more_specific));

  Ctor *first = ctors.front(), *second = 0;
  ctors.pop_front();
  if(ctors.size() > 0){
    second = ctors.front();
    ctors.push_front(second);
  }
  ctors.push_front(first);

  if(second and (second->more_specific(first) or !first->more_specific(second)))
    return 0;
  return first;
}

void CInvoke::encode() const{
  std::cout << "# constructor invocation args\n"
    "\tmovl   8(%ebp), %eax\n"
    "\tpushl  %eax\n"
    ;
  std::for_each(args->begin(), args->end(), std::mem_fun(&Expr::encode));
  std::cout << "# constructor invocation work\n"
    "\tpushl  $" << (args->size()+1) << "\n"
    "\tcall   _TRUNTIME_rargs\n"
    "\tpopl   %ecx\n"
    "\tpopl   %eax\n"
    "\tpushl  %eax\n"
    "\tmovl   (%eax), %eax\n"
    "\tcall   " << ctor->munge() << "\n"
    "#\tpopl   %eax\n"
    "\taddl   $" << (4*(args->size()+1)) << ", %esp\n"
    "#\tpushl  %eax\n"
    ;
}

bool CInvoke::calls(const Ctor *c) const{
  const CInvoke *next = this;
  while(next){
    if(!next->ctor) return false;
    if(next->invoker == This and next->ctor->sig_equals(c))
      return true;
    next = dynamic_cast<CInvoke*>(next->ctor->body->stats->front());
  }
  return false;
}
