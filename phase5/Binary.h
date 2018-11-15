#ifndef _BINARY_DOT_H_
#define _BINARY_DOT_H_

#include "Expr.h"

/** Abstract binary operation. Overridden Node functions call themselves on 
* left expression, then right expression
*/
class Binary : public Expr{
  Expr *lhs, /**< left operand */
    *rhs;    /**< right operand */
protected:
  /** Re-assign lhs */
  void left(Expr *e);
  /** Re-assign rhs */
  void right(Expr *e);
public:
  /** Operation between left and right */
  Binary(Expr *left, Expr *right);
  ~Binary();
  /** Returns true iff left->type_equal(right) */
  bool types_match();
  /** Returns true iff left and right are compile-time constants */
  bool is_constexpr();
  /** Calls error(), reporting that left and right's types differ */
  void mismatch_error(const char *name, const Type *expected);
  /** Returns the left-hand side expression */
  Expr *left() const;
  /** Returns the right-hand side expression */
  Expr *right() const;
  virtual void dump() const = 0;
  virtual Node *analyze();
  virtual void encode() const = 0;
};

#endif
