#ifndef _CASTA_DOT_H_
#define _CASTA_DOT_H_

/* Steve McCoy */

#include "Expr.h"

class CastA : public Expr{
  Type *target;
  Expr *sub;
  bool runtime;
public:
  CastA(Type *t, Expr *s);
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
