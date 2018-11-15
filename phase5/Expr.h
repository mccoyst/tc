#ifndef _EXPR_DOT_H_
#define _EXPR_DOT_H_

#include "Node.h"
#include "Type.h"

/** Abstract expression. All derived classes have a Type* member, @a type.
* The type is usually determined during analysis.
*/
class Expr : public Node{
  /** The expression's type */
  const Type *typ;
protected:
  /** Initializes expression's type to the unknown type. */
  Expr();
public:
  /** Returns true iff this expression has the same type as e */
  bool type_equal(const Expr *e) const;
  /** Returns true iff this expression's type is the same as t */
  bool type_equal(const Type *t) const;
  /** Returns the expression's type */
  const Type *type() const;
  /** Sets the expression's type */
  void type(const Type *t);
};

#endif
