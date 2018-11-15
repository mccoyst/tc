#ifndef _DIV_DOT_H_
#define _DIV_DOT_H_

/*Steve McCoy
*/

#include "Binary.h"

/** Division operator.
*/
class Div : public Binary{
public:
  /** left / right */
  Div(Expr *left, Expr *right);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
