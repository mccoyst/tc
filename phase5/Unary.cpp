/*Steve McCoy
*/

#include <cassert>
#include <typeinfo>
#include "Unary.h"
#include "IntLit.h"

Unary::Unary(Expr *s)
  : sub(s){
  assert(sub != 0);
}

Unary::~Unary(){
  delete sub;
}

bool Unary::is_constexpr() const{
  return typeid(*subexpr()) == typeid(IntLit);
}

void Unary::mismatch_error(const std::string& nm, const Type *ex){
  error("'" + nm + "' expects operand of type " + ex->str() + " and got " + subexpr()->type()->str());
}

Expr *Unary::subexpr() const{
  return sub;
}

void Unary::subexpr(Expr *e){
  sub = e;
}

void Unary::dump() const{
  subexpr()->dump();
}

Node *Unary::analyze(){
  subexpr(dynamic_cast<Expr*>(sub->analyze()));
  type(subexpr()->type());
  return this;
}

void Unary::encode() const{
  subexpr()->encode();
}
