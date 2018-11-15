/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include <sstream>
#include "Cast.h"
#include "Var.h"

Cast::Cast(Expr *type, Expr *expr)
  : pexpr(type), expr(expr), runtime(false){
  assert(pexpr != 0 && expr != 0);
}

void Cast::dump() const{
  std::cerr << " cast from " << *expr->type() << " to " << *pexpr->type() << '\n';
  pexpr->dump();
  expr->dump();
  std::cerr << " end cast\n";
}

Node *Cast::analyze(){
  expr = dynamic_cast<Expr*>(expr->analyze());
  assert(expr != 0);
  Var *v = dynamic_cast<Var*>(pexpr);
  if(!v)
    error("Invalid expression in cast");
  const Type *t = Type::user(v->name());
  type(t);
  v->type(t);
  if(!t->defined())
    error("Cannot cast to undefined type " +t->str());
  else if(t->incomplete())
    error("Cannot cast to incomplete type " + t->str());
  else if(t->is(expr->type()) or expr->type()->can_upcast(t))
    ; // static cast
  else if(expr->type()->can_downcast(t))
    runtime = true;
  else
    error("Invalid conversion from " + expr->type()->str() + " to " + t->str());
  return this;
}

void Cast::encode() const{
  expr->encode();
  if(runtime)
    std::cout << "# dynamic cast\n"
      "\tpushl $" << type()->str() << "$VMT\n"
      "\tpushl $" << line() << "\n"
      "\tcall  _TRUNTIME_cast\n"
      "\taddl  $8, %esp\n"
      ;
  else
    std::cout << "# static cast\n";
}
