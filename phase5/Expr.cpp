/*Steve McCoy
*/

#include "Expr.h"

Expr::Expr()
  : typ(Type::unknown()){
}

bool Expr::type_equal(const Expr *that) const{
  return this->type()->is(that->type());
}

bool Expr::type_equal(const Type *t) const{
  return type()->is(t);
}

const Type *Expr::type() const{
  return typ;
}

void Expr::type(const Type *t){
  typ = t;
}
