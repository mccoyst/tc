#ifndef _SUB_DOT_H_
#define _SUB_DOT_H_

/*Steve McCoy
*/

#include "Binary.h"

/** Subtraction operator.
*/
class Sub : public Binary{
public:
  /** left - right */
  Sub(Expr *left, Expr *right);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
