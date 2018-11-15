/*Steve McCoy
* Some MAJOR Typeactoring is in order.
*/

#include <cassert>
#include <algorithm>
#include <functional>
#include <iostream>
#include "CDecl.h"
#include "FDecl.h"
#include "SymTab.h"
using namespace std;

bool dump_classes;

namespace{
  bool non_ref(const Type *t){
    return t->is(Type::int_type()) or t->is(Type::null()) or t->is(Type::unknown());
  }
}

CDecl::CDecl(Ident name, Type *super, std::list<CBDecl*> *body)
  : name(name), super(super),body(body), good(true){
  assert(this->super != 0 and body != 0);

  type = Type::user(name);
  if(type->defined()){
    error("Redefinition of class " + name.str(), Severity::Problem);
    good = false;
    return;
  }
  type->defined(true);

  if(non_ref(super)){
    error("Cannot inherit from " + super->str(), Severity::Problem);
    good = false;
    return;
  }
  if(super->is(type) or super->inherits_from(type)){
    error("Class " + name.str() + " inherits from itself", Severity::Problem);
    good = false;
    return;
  }

  type->super(super);
  super->register_subclass(this);
  add_members();
  if(super->incomplete()) return;
  type->set_offsets();
  type->complete(true);
}

void CDecl::dump() const{
  cerr << "Declaration of class " << name
    << " (" << (Type::user(name)->incomplete() ? "incomplete" : " complete ")
    << "):\n";
  for_each(body->begin(),body->end(), mem_fun(&CBDecl::dump));
  cerr << "End " << name << '\n';
}

typedef list<CBDecl*>::iterator CBit;

Node *CDecl::analyze(){
  if(!good) return this;
  update();
  Type::Make_current mc(type);
  for(CBit d = body->begin(); d != body->end(); ++d){
    try{
      (*d)->analyze();
    }catch(SyntaxError& s){
      good = false;
    }
  }
  return this;
}

void CDecl::update(){
  if(!good) return;
  if(!super->incomplete()){
    type->set_offsets();
    type->complete(true);
  }else // shouldn't get here
    error("Derivation of "+name.str()+" from undefined type "
    +super->str(), Severity::Problem);
}

void CDecl::add_members(){
  Type::current(type);
  for(CBit d = body->begin(); d != body->end(); ++d)
    try{
      (*d)->add_to(type);
//      (*d)->type->register_subclass(this); // should be register_dependency
    }catch(SyntaxError& e){ /* go on */ }
  Type::current(0);
}

void CDecl::encode() const{
  type->encode();
  Type::Make_current mc(type);
  for_each(body->begin(), body->end(), mem_fun(&CBDecl::encode));
}
