#ifndef _AACCESS_DOT_H_
#define _AACCESS_DOT_H_

/* Steve McCoy */

#include "Binary.h"

class AAccess : public Binary{
  friend class Ass;
public:
  AAccess(Expr *l, Expr *r);
  void dump() const;
  Node *analyze();
  void encode() const;
  Expr *left() const{ return Binary::left(); }
  Expr *right() const{ return Binary::right(); }
};

#endif
