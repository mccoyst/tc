/*Steve McCoy
*/

#include <cassert>
#include <algorithm>
#include <functional>
#include <iostream>
#include "FDecl.h"

FDecl::FDecl(Type *type, std::list<VDecl*> *decls)
  : type(type), decls(decls){
  assert(type != 0 && decls != 0);
}

void FDecl::dump() const{
  std::cerr << " Field declaration of type " << type->str() << ": [";
  std::for_each(decls->begin(), decls->end(), std::mem_fun(&VDecl::dump));
  std::cerr << "]\n";
}

Node *FDecl::analyze(){
  if(!type->defined())
    error("Class " + type->str() + " is undefined");
  if(type->incomplete())
    error("Class " + type->str() + " is incomplete");
  return this;
}

void FDecl::encode() const{
}

inline Type *merge_if_array(Type *t, const VDecl *vd){
  return (vd->dimensions() == 0) ? t : Type::array(t, vd->dimensions());
}

void FDecl::add_to(Type *t){
  std::list<VDecl*>::iterator v;
  for(v = decls->begin(); v != decls->end(); ++v){
    VDecl *vd = *v;
    if(!t->field(vd->name(),merge_if_array(type,vd)))
      error("Field "+vd->name().str()
      +" is already declared in class "+t->str(), Severity::Problem);
  }
}

bool FDecl::complete() const{
  return !type->incomplete();
}

std::string FDecl::str() const{
  return type->str();
}
