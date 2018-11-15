#ifndef _NOT_DOT_H_
#define _NOT_DOT_H_

/*Steve McCoy
*/

#include "Unary.h"

/** Logical negation operator.
*/
class Not : public Unary{
public:
  /** !sub */
  explicit Not(Expr *sub);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
