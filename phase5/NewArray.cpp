/* Steve McCoy */

#include <iostream>
#include <algorithm>
#include <functional>
#include "NewArray.h"

NewArray::NewArray(const Type *t, std::list<Expr*> *e, uint d)
  : elem(t), dimexprs(e), dims(d){
}

void NewArray::dump() const{
  std::cerr << ' ' << (dimexprs->size()+dims) << "-D " << *elem << " Array creation expression\n";
}

inline Expr *dim_analyze(Expr *e){
  Expr *r = 0;
  try{
    r = dynamic_cast<Expr*>(e->analyze());
  }catch(SyntaxError& e){
    // continue
  }
  if(r and !r->type()->is(Type::int_type()))
    r->error("Dimension expression is of type "+r->type()->str()+" and not type int");
  return r;
}

Node *NewArray::analyze(){
  if(!elem->defined())
    error("Cannot instantiate array of undefined type '"+elem->str()+"'");
  if(elem->incomplete())
    error("Cannot instantiate array of incomplete type '"+elem->str()+"'");
  std::transform(dimexprs->begin(),dimexprs->end(),dimexprs->begin(), dim_analyze);
  type(Type::array(const_cast<Type*>(elem), dimexprs->size()+dims));
  return this;
}

void NewArray::encode() const{
  std::cout << "# array creation\n";
  std::cout << "#  dimension expressions:\n";
  std::for_each(dimexprs->begin(),dimexprs->end(), std::mem_fun(&Expr::encode));
  std::cout <<
    "#  number of dimension expressions:\n"
    "\tpushl   $" << dimexprs->size() << "\n"
    "#  additional dimensions:\n"
    "\tpushl   $" << dims << "\n"
    "#  element VMT:\n"
    "\tpushl   $";
  if(elem->is(Type::int_type()))
    std::cout << "0\n";
  else
    std::cout << *elem << "$VMT\n";
  std::cout <<
    "#  line number:\n"
    "\tpushl   $" << line() << "\n"
    "#  real work:\n"
    "\tcall    _TRUNTIME_new_array\n"
    "\taddl    $" << (4*4 + dimexprs->size()*4) << ", %esp\n"
    "\tpushl   %eax\n"
    ;
}
