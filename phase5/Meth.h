#ifndef _METH_DOT_H_
#define _METH_DOT_H_

/*Steve McCoy
*/

#include "CBDecl.h"
#include "Param.h"
#include "Function.h"
#include "Block.h"

class Meth : public CBDecl, public Function{
public:
  Meth(Type *ret, Ident name, std::list<Param*> *params, Block *body);
  void dump() const;
  Node *analyze();
  void encode() const;
  void add_to(Type *t);

  static Meth *object_equals();
};

#endif
