#ifndef _CAST_DOT_H_
#define _CAST_DOT_H_

/*Steve McCoy
*/

#include "Expr.h"

/** Explicit cast of expression to type.
*/
class Cast : public Expr{
  Expr *pexpr, *expr;
  bool runtime;
public:
  Cast(Expr *type, Expr *expr);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
