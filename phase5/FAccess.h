#ifndef _FACCESS_DOT_H_
#define _FACCESS_DOT_H_

/*Steve McCoy
*/

#include "Expr.h"
#include "Ident.h"

/** Field access.
*/
class FAccess : public Expr{
  Expr *left;
  Ident field;
public:
  FAccess(Expr *left, Ident field);
  void dump() const;
  Node *analyze();
  void encode() const;
  Ident field_name() const{ return field; }
  Expr *expr() const{ return left; }
};

#endif
