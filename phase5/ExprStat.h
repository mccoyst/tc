#ifndef _EXPRSTAT_DOT_H_
#define _EXPRSTAT_DOT_H_

/*Steve McCoy
*/

#include "Expr.h"
#include "Stat.h"

/** Statement of one expression.
*/
class ExprStat : public Stat{
  Expr *expr;
public:
  /** Statement of one expression, expr */
  explicit ExprStat(Expr *expr);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
