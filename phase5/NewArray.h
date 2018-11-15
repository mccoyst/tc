#ifndef  _NEWARRAY_DOT_H_
#define  _NEWARRAY_DOT_H_

#include "Expr.h"
#include <list>

class NewArray : public Expr{
  const Type *elem;
  std::list<Expr*> *dimexprs;
  uint dims;
public:
  NewArray(const Type *t, std::list<Expr*> *e, uint d);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
