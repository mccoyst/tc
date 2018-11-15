#ifndef _UNARY_DOT_H_
#define _UNARY_DOT_H_

#include "Expr.h"

/** Abstract unary operation. Overridden Node functions call themselves on the subexpression.
*/
class Unary : public Expr{
  /** subexpression */
  Expr *sub;
protected:
  /** sets subexpr */
  void subexpr(Expr *sub);
public:
  /** Operate on sub */
  explicit Unary(Expr *sub);
  ~Unary();
  /** Returns true if sub is a compile-time constant */
  bool is_constexpr() const;
  /** Calls error(), explaining that sub isn't of the type expected */
  void mismatch_error(const std::string& name, const Type *expected);
  /** Returns the operator's subexpression. */
  Expr *subexpr() const;
  virtual void dump() const = 0;
  virtual Node *analyze() = 0;
  virtual void encode() const = 0;
};

#endif
