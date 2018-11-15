#ifndef _EQUALS_DOT_H_
#define _EQUALS_DOT_H_

/*Steve McCoy
*/

#include "Binary.h"

/** Equality operator.
*/
class Equals : public Binary{
public:
  /** left == right */
  Equals(Expr *left, Expr *right);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
