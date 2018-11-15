/* Steve McCoy */

#include "Type.h"

const Type *cur = 0;

void Type::current(const Type *t){
  cur = t;
}

const Type *Type::current(){
  return cur;
}
