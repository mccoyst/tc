#ifndef _DEREF_DOT_H_
#define _DEREF_DOT_H_

/*Steve McCoy
*/

#include "Unary.h"

/** Dereference.
*/
class Deref : public Unary{
public:
  /** Dereference sub. */
  explicit Deref(Expr *sub);
  void dump() const;
  Node *analyze();
  void encode() const;
  /** yields ownership of subexpr */
  void deref();
};

#endif
