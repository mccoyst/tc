#ifndef _NEW_DOT_H_
#define _NEW_DOT_H_

/*Steve McCoy
*/

#include <list>
#include "Expr.h"
#include "Ctor.h"

/** Class instance creation expression.
*/
class New : public Expr{
  std::list<Expr*> *args;
  Ctor *ctor;
  bool match_constructor();
public:
  New(const Type *type, std::list<Expr*> *args);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
