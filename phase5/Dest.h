#ifndef _DEST_DOT_H_
#define _DEST_DOT_H_

#include <list>
#include "Ident.h"
#include "CBDecl.h"
#include "Block.h"
#include "Function.h"

class Dest : public CBDecl, public Function{
public:
  Dest(Ident,Block*);
  void dump() const;
  Node *analyze();
  void encode() const;
  void add_to(Type *t);
};

#endif
