#ifndef _NEG_DOT_H_
#define _NEG_DOT_H_

/*Steve McCoy
*/

#include "Unary.h"

/** Mathematical negation operator.
*/
class Neg : public Unary{
public:
  /** -sub */
  explicit Neg(Expr *sub);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
