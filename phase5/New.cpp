/*Steve McCoy
*/

#include <algorithm>
#include <functional>
#include <cassert>
#include <iostream>
#include "New.h"
#include "Ctor.h"
#include "CInvoke.h"
#include "utility.h"

New::New(const Type *t, std::list<Expr*> *args)
  : args(args){
  assert(t != 0 && args != 0);
  type(t);
}

void New::dump() const{
  std::cerr << " New of " << type()->str() << '\n';
  std::for_each(args->begin(), args->end(), std::mem_fun(&Expr::dump));
  std::cerr << " End new\n";
}

Node *New::analyze(){
  if(type()->is(Type::unknown()))
    error("Cannot instantiate unknown type");
  if(type()->is(Type::int_type()))
    error("Cannot call new on type " + type()->str());
  else if(type()->incomplete())
    error("Cannot call new on incomplete type " + type()->str());
  std::transform(args->begin(), args->end(), args->begin(),
    dc_fun<Expr>(std::mem_fun(&Expr::analyze)));

  std::list<Ctor*> candidates;
  type()->applicable_ctors(candidates, args);
  if(candidates.size() == 0){
    if(args->size() > 0)
      error("No matching constructor for call to new "+type()->str());
    else{
      ctor = type()->noarg();
      if(!ctor) error("No matching constructor for call to new "+type()->str());
    }
  }else{
    ctor = CInvoke::most_specific(candidates);
    if(!ctor) error("No most-specific constructor for call to new "+type()->str());
  }
  return this;
}

void New::encode() const{
  Type::Make_current mc(type());
  std::cout << "# new args " << type()->str() << '\n';
  std::for_each(args->begin(), args->end(), std::mem_fun(&Expr::encode));
  std::cout << "# new work\n"
    "\tpushl  $" << args->size() << "\n"
    "\tcall   _TRUNTIME_rargs\n"
    "\tpopl   %ecx\n"
    "\tpushl  $" << line() << "\n"
    "\tpushl  $" << (1+type()->words()) << "\n"
    "\tcall   _TRUNTIME_new\n"
    "\taddl   $8, %esp\n"
    "\tmovl   $" << type()->str() << "$VMT, (%eax)\n"
    "\tpushl  %eax\n"
    "\tcall   " << ctor->munge() << "\n"
    "\tpopl   %eax\n"
    "\taddl   $" << (args->size()*4) << ", %esp\n"
    "\tpushl  %eax\n"
    ;
}
