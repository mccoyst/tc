#ifndef _INTLIT_DOT_H_
#define _INTLIT_DOT_H_

/*Steve McCoy
*/

#include "Expr.h"

/** Integer-value literal.
*/
class IntLit : public Expr{
  const uint val;
public:
  /** literal with value value */
  explicit IntLit(uint value);
  /** the value of the literal */
  uint value() const{ return val; }
  void dump() const;
  Node *analyze();
  void encode() const;
  /** true if the node's parent is a Neg, false otherwise */
  bool is_negated;    // I don't like this
};

/** Function that folds a constant unary expression, using unop.
*/
template<class Op>
IntLit *static_unop(Expr *sub, Op unop){
  return new IntLit(
    unop(dynamic_cast<IntLit*>(sub)->value())
  );
}

/** Function that folds a constant binary expression, using binop.
*/
template<class Op>
IntLit *static_binop(Expr *left, Expr *right, Op binop){
  return new IntLit(
    binop(dynamic_cast<IntLit*>(left)->value(),
          dynamic_cast<IntLit*>(right)->value())
  );
}

#endif
