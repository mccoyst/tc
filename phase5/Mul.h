#ifndef _MUL_DOT_H_
#define _MUL_DOT_H_

/*Steve McCoy
*/

#include "Binary.h"

/** Multiplication opeartor.
*/
class Mul : public Binary{
public:
  /** left * right */
  Mul(Expr *left, Expr *right);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
