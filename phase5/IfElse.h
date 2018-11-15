#ifndef _IFELSE_DOT_H_
#define _IFELSE_DOT_H_

/*Steve McCoy
*/

#include <stack>
#include "Control.h"

/** If-then-else statement. @bug stack behaviors are sloppy
*/
class IfElse : public Control{
  Stat *ifstat, *elstat;
  static uint depth;
  static std::stack<uint> stack;
  static void push();
  static void pop();
public:
  /** if(cond) ifstat else elstat */
  IfElse(Expr *cond, Stat *ifstat, Stat *elstat);
  ~IfElse();
  void dump() const;
  Node *analyze();
  void encode() const;
  /** Prefix for if-else assembly labels */
  static const char *LblPrefix;
  /** Value of current first if-else label */
  static uint label1();
  /** Value of current second if-else label */
  static uint label2();
};

#endif
