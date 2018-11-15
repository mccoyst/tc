/*Steve McCoy
*/

#include "MInvoke.h"
#include "Meth.h"
#include "Deref.h"
#include "Super.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>
#include <typeinfo>

MInvoke::MInvoke(Expr *l, Ident n, std::list<Expr*> *a)
  : left(l), name(n), args(a), offset(-1), invoke_super(false){
  assert(left != 0 and args != 0);
}

void MInvoke::dump() const{
  std::cerr << " Function invocation " << type()->str() << ' ' << name << '(';
  std::list<Expr*>::const_iterator a = args->begin();
  for(; a != args->end(); ++a)
    std::cerr << (*a)->type()->str() << ", ";
  std::cerr << ")\n";
}

Node *MInvoke::analyze(){
  left = dynamic_cast<Expr*>(left->analyze());
  std::list<Expr*>::iterator a;
  for(a = args->begin(); a != args->end(); ++a)
    try{
      *a = dynamic_cast<Expr*>((*a)->analyze());
    }catch(SyntaxError& s){ /* go on */ }

  // if doing a super call, the method is known at compile-time
  if(typeid(*left) == typeid(*super_expr())) invoke_super = true;

  std::list<Meth*> candidates;
  left->type()->applicable_meths(candidates, name, args);
  if(candidates.size() == 0)
    error("No matching method for call to " + name.str());
  meth = most_specific(candidates);
  offset = left->type()->meth_offset(meth);
  type(meth->rtype());
  return this;
}

Meth *MInvoke::most_specific(std::list<Meth*>& meths){
  meths.sort(std::mem_fun(&Meth::more_specific));

  Meth *first = meths.front(), *second = 0;
  meths.pop_front();
  if(meths.size() > 0)
    second = meths.front();
  meths.push_front(first);

  if(second and (second->more_specific(first) or !first->more_specific(second)))
    error("No most-specific method for call to " + name.str());
  return first;
}

void MInvoke::encode() const{
  left->encode();
  std::for_each(args->begin(), args->end(), std::mem_fun(&Expr::encode));
  std::cout << "# method invocation\n"
    "\tpushl  $" << (args->size()+1) << "\n"
    "\tcall   _TRUNTIME_rargs\n"
    "\tpopl   %ecx\n"
    "\tpushl  $" << line() << "\n"
    "\tcall   _TRUNTIME_check_null\n"
    "\tpopl   %ecx\n"
    "\tpopl   %eax\n"
    "\tpushl  %eax\n"
    ;
  if(invoke_super) std::cout <<
    "# super method\n"
    "\tcall   " << meth->munge() << "\n"
    ;
  else std::cout <<
    "# runtime method\n"
    "\tmovl   (%eax), %eax\n"
    "\taddl   $" << (offset*4) << ", %eax\n"
    "\tmovl   (%eax), %eax\n"
    "\tcall   *%eax\n"
    ;
  std::cout <<
    "\taddl   $" << ((args->size()+1)*4) << ", %esp\n"
    "\tpushl  %eax\n"
    ;
}
