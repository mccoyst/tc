#ifndef _ASS_DOT_H_
#define _ASS_DOT_H_

/*Steve McCoy
*/

#include "Binary.h"

/** Assignment operator.
*/
class Ass : public Binary{
  bool inserting;
public:
  /** assign right to left */
  Ass(Expr *left, Expr *right);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
