#ifndef _MINVOKE_DOT_H_
#define _MINVOKE_DOT_H_

#include "Expr.h"
#include "Meth.h"

class MInvoke : public Expr{
  Expr *left;
  Ident name;
  std::list<Expr*> *args;
  Meth *most_specific(std::list<Meth*>&);
  int offset;
  bool invoke_super;
  Meth *meth;
public:
  MInvoke(Expr *left, Ident name, std::list<Expr*> *args);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
