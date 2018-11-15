/*Steve McCoy*/

#include "Function.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

const Function *Function::cur = 0;

Function::Function(Type *r, Ident n, std::list<Param*> *p, Block *b)
  : ret(r), name(n), params(p), body(b){
}

const Type *Function::rtype() const{
  return ret;
}

Ident Function::ident() const{
  return name;
}

const Type *Function::param_type(Ident id) const{
  std::list<Param*>::const_iterator i, e;
  for(i = params->begin(), e = params->end(); i != e; ++i)
    if((*i)->name() == id) return (*i)->type();
  return 0;
}

int Function::nparams() const{
  return params->size();
}

int Function::param_pos(Ident id) const{
  int p = 0;
  std::list<Param*>::const_iterator i, e;
  for(i = params->begin(), e = params->end(); i != e; ++i, ++p)
    if((*i)->name() == id) return p;
  return -1;
}

inline bool assignment_convertible_arg(const Expr *arg, const Param *param){
  return arg->type()->can_upcast(param->type());
}

bool Function::matches(const std::list<Expr*> *args) const{
  if(args->size() != params->size())
    return false;
  return std::equal(args->begin(), args->end(), params->begin(),
    assignment_convertible_arg);
}

inline bool assignment_convertible_param(const Param *a, const Param *b){
  return a->type()->can_upcast(b->type());
}

bool Function::more_specific(const Function *that) const{
  if(that->params->size() != this->params->size())
    return false;
  return std::equal(params->begin(), params->end(), that->params->begin(),
    assignment_convertible_param);
}

inline bool param_type_equal(const Param *a, const Param *b){
  return a->type()->is(b->type());
}

bool Function::sig_equals(const Function *that) const{
  if(this->name != that->name) return false;
  if(this->params->size() != that->params->size()) return false;

  return std::equal(params->begin(), params->end(), that->params->begin(),
    param_type_equal);
}

void Function::dump(bool everything) const{
  Function::Make_current mc(this);
  if(rtype()) std::cerr << *rtype() << ' ';
  std::cerr << name << "(";
  std::for_each(params->begin(), params->end(), std::mem_fun(&Param::dump));
  std::cerr << ");\n";
  if(everything) body->dump();
}

inline Param *analyze_noexcept(Param *p){
  try{
    return dynamic_cast<Param*>(p->analyze());
  }catch(...){
    return p;
  }
}

void Function::analyze_params(){
  std::for_each(params->begin(),params->end(), analyze_noexcept);
}

void Function::analyze_body(){
  Function::Make_current mc(this);
  body->analyze();
}

void Function::encode_body() const{
  Function::Make_current mc(this);
  body->encode();
}

inline bool param_names_equal(const Param *a, const Param *b){
  return a->name() == b->name();
}

bool Function::duplicated_param_name(Ident& out) const{
  std::list<Param*> copy = *params;
  copy.sort();
  std::list<Param*>::const_iterator p = std::adjacent_find(copy.begin(),copy.end(),
    param_names_equal);
  if(p != copy.end()){
    out = (*p)->name();
    return true;
  }
  return false;
}

struct munge_param{
  std::ostream& o;
  munge_param(std::ostream& os) : o(os) {}
  void operator ()(const Param *p){
    o << '$' << p->type()->str();
  }
};

std::string Function::munge() const{
  std::ostringstream os;
  os << enclosing->str() << '$' << name;
  std::for_each(params->begin(),params->end(), munge_param(os));
  return os.str();
}
