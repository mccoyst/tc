#ifndef _VALRETURN_DOT_H_
#define _VALRETURN_DOT_H_

/*Steve McCoy
*/

#include "Control.h"

/** Return a value from function.
*/
class Return : public Control{
  bool in_main;
public:
  /** return val */
  explicit Return(Expr *val);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
