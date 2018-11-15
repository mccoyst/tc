/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Equals.h"
#include "IntLit.h"

Equals::Equals(Expr *l, Expr *r)
  : Binary(l, r){
}

void Equals::dump() const{
  std::cerr << " Equality operator " << *type() << '\n';
  Binary::dump();
}

Node *Equals::analyze(){
  extern bool optimizing;
  Binary::analyze();
  type(Type::int_type());
  if(!types_match())
    mismatch_error("==", left()->type());
  if(optimizing and is_constexpr()){
  	std::auto_ptr<Equals> cleaner(this);
  	return static_binop(left(),right(), std::equal_to<int>());
  }
  return this;
}

void Equals::encode() const{
  Binary::encode();
  std::cout << "# Equality\n"
    "\tpopl   %edx\n"
    "\tpopl   %eax\n"
    "\tcmpl   %eax, %edx\n"
    "\tsete   %al\n"
    "\tmovzbl %al, %eax\n"
    "\tpushl  %eax\n"
    ;
}
