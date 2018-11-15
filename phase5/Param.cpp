/*Steve McCoy
*/

#include "Param.h"
#include <cassert>
#include <iostream>

Param::Param(const Type *t, Ident i, uint dimensions)
  : typ(t), id(i), dims(dimensions){
  assert(typ != 0);
}

void Param::dump() const{
  std::cerr << type()->str() << ' ' << name() << ", ";
}

Node *Param::analyze(){
  if(!type()->defined())
    error("Class " + type()->str() + " is undefined");
  if(type()->incomplete())
    error("Class " + type()->str() + " is incomplete");
  return this;
}

// probably shouldn't get called
void Param::encode() const{
  std::cout << "# parameter\n";
}

const Type *Param::type() const{
  return typ;
}

Ident Param::name() const{
  return id;
}
