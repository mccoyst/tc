#ifndef _CTOR_DOT_H_
#define _CTOR_DOT_H_

/*Steve McCoy
*/

#include "CBDecl.h"
#include "Block.h"
#include "Param.h"
#include "Function.h"
#include "Expr.h"

class Ctor : public CBDecl, public Function{
  friend class CInvoke;
  bool implicit_superctor;
public:
  Ctor(Type *name, std::list<Param*> *params, Block *body);
  void dump() const;
  Node *analyze();
  void encode() const;
  void add_to(Type *t);

  static Ctor *object();
};

#endif
