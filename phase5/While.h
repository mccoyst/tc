#ifndef _WHILE_DOT_H_
#define _WHILE_DOT_H_

/*Steve McCoy
*/

#include <stack>
#include "Control.h"

/** while loop. @bug stack behaviors are sloppy
*/
class While : public Control{
  Stat *stat;
  uint entry, exit;
  static uint depth;
  static std::stack<uint> stack;
  static void push();
  static void pop();
public:
  /** while(cond) stat */
  While(Expr *cond, Stat *stat);
  ~While();
  void dump() const;
  Node *analyze();
  void encode() const;
  /** The prefix for assembly while labels. */
  static const char *LblPrefix;
  /** Returns true if currently analyze()ing inside of a while. */
  static bool inside();
  /** Returns the current entry label number. */
  static uint label1();
  /** Returns the current exit label number. */
  static uint label2();
};

#endif
