#ifndef _DEL_DOT_H_
#define _DEL_DOT_H_

/*Steve McCoy
*/

#include "Expr.h"
#include "Stat.h"

/** Delete statement.
*/
class Del : public Stat{
  Expr *expr;
public:
  Del(Expr *expr);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
