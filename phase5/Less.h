#ifndef _LESS_DOT_H_
#define _LESS_DOT_H_

/*Steve McCoy
*/

#include "Binary.h"

/** Less-than operator.
*/
class Less : public Binary{
public:
  /** left < right */
  Less(Expr *left, Expr *right);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
