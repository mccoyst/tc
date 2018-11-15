/* Steve McCoy */

#include "CastA.h"
#include <cassert>
#include <iostream>

CastA::CastA(Type *t, Expr *s)
  : target(t), sub(s){
}

void CastA::dump() const{
  std::cerr << " Cast to array type '" << target->str() << "'\n";
  sub->dump();
  std::cerr << " End cast\n";
}

Node *CastA::analyze(){
  sub = dynamic_cast<Expr*>(sub->analyze());
  assert(sub != 0);
  if(!target->defined())
    error("Cannot cast to undefined type " +target->str());
  else if(target->incomplete())
    error("Cannot cast to incomplete type " + target->str());
  else if(sub->type()->can_upcast(target))
    ; // static cast
  else if(sub->type()->can_downcast(target))
    runtime = true;
  else
    error("Invalid conversion from " + sub->type()->str() + " to " + target->str());
  type(target);
  return this;
}

void CastA::encode() const{
  sub->encode();
  if(runtime){
    std::cout << "# dynamic cast to array\n"
      "\tpushl  $" << target->dimensions() << "\n"
      "\tpushl  $";
    if(target->element()->is(Type::int_type()))
      std::cout << 0 << '\n';
    else
      std::cout << *target->element() << "$VMT\n";
    std::cout <<
      "\tpushl  $" << line() << "\n"
      "\tcall   _TRUNTIME_array_cast\n"
      "\taddl   $12, %esp\n"
      ;
  }else
    std::cout << "# static cast to array\n";
}
