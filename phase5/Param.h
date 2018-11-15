#ifndef _PARAM_DOT_H_
#define _PARAM_DOT_H_

/*Steve McCoy
*/

#include "Node.h"
#include "Type.h"
#include "Ident.h"

class Param : public Node{
  const Type *typ;
  Ident id;
  uint dims;
public:
  Param(const Type *typ, Ident id, uint dimensions);
  void dump() const;
  Node *analyze();
  void encode() const;
  const Type *type() const;
  Ident name() const;
  uint dimensions() const{ return dims; }
  //void add_dimension(){ ++dims; }
};

struct param_less{
  bool operator()(const Param *a, const Param *b){
    return a->name() < b->name();
  }
};

#endif
