/*Steve McCoy
*/

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include "Add.h"
#include "IntLit.h"

Add::Add(Expr *l, Expr *r)
  : Binary(l, r){
}

void Add::dump() const{
  std::cerr << " Addition " << *type() << '\n';
  Binary::dump();
}

Node *Add::analyze(){
  extern bool optimizing;
  Binary::analyze();
  if(!type_equal(Type::int_type()) or !types_match())
    mismatch_error("+", Type::int_type());
  if(optimizing and is_constexpr()){
    std::auto_ptr<Add> cleaner(this);
    return static_binop(left(),right(), std::plus<int>());
  }
  return this;
}

void Add::encode() const{
  Binary::encode();
  std::cout << "# Addition\n"
    "\tpopl   %edx\n"
    "\tpopl   %eax\n"
    "\taddl   %edx, %eax\n"
    "\tpushl  %eax\n"
    ;
}
