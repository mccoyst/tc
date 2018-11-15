/*Steve McCoy
*/

#include <cassert>
#include <sstream>
#include "Binary.h"
#include "IntLit.h"

Binary::Binary(Expr *l, Expr *r)
  : lhs(l), rhs(r){
  assert(lhs != 0 && rhs != 0);
}

Binary::~Binary(){
  delete lhs; delete rhs;
}

bool Binary::types_match(){
  return left()->type_equal(right()) or right()->type()->can_upcast(left()->type());
}

bool Binary::is_constexpr(){
  return typeid(*left()) == typeid(IntLit)
    && typeid(*right()) == typeid(IntLit);
}

void Binary::mismatch_error(const char *nm, const Type *ex){
  std::ostringstream os;
  os << "'" << nm << "' expects operands of type " << *ex
    << " and got " << *left()->type() << ',' << *right()->type();
  error(os.str());
}

void Binary::dump() const{
  left()->dump();
  right()->dump();
}

Expr *Binary::left() const{
  return lhs;
}

void Binary::left(Expr *l){
  lhs = l;
}

Expr *Binary::right() const{
  return rhs;
}

void Binary::right(Expr *r){
  rhs = r;
}

Node *Binary::analyze(){
  left(dynamic_cast<Expr*>(left()->analyze()));
  right(dynamic_cast<Expr*>(right()->analyze()));
  assert(left() != 0 && right() != 0);
  type(left()->type());
  return this;
}

void Binary::encode() const{
  left()->encode();
  right()->encode();
}
