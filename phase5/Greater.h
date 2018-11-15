#ifndef _GREATER_DOT_H_
#define _GREATER_DOT_H_

/*Steve McCoy
*/

#include "Binary.h"

/** Greater-than operator.
*/
class Greater : public Binary{
public:
  /** left > right */
  Greater(Expr *left, Expr *right);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
