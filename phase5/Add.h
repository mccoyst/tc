#ifndef _ADD_DOT_H_
#define _ADD_DOT_H_

/*Steve McCoy
*/

#include "Binary.h"

/** Addition operator.
*/
class Add : public Binary{
public:
  /** left + right */
  Add(Expr *left, Expr *right);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
