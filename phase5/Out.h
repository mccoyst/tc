#ifndef _OUT_DOT_H_
#define _OUT_DOT_H_

/*Steve McCoy
*/

#include "Expr.h"
#include "Stat.h"

/** Output statement.
*/
class Out : public Stat{
  Expr *sub;
public:
  /** out sub; */
  explicit Out(Expr *sub);
  ~Out();
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
