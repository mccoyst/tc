/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Mul.h"
#include "IntLit.h"

Mul::Mul(Expr *l, Expr *r)
  : Binary(l, r){
}

void Mul::dump() const{
  std::cerr << " Multiplication\n";
  Binary::dump();
}

Node *Mul::analyze(){
  extern bool optimizing;
  Binary::analyze();
  if(!type_equal(Type::int_type()) or !types_match())
  	mismatch_error("*", Type::int_type());
  if(optimizing and is_constexpr()){
  	std::auto_ptr<Mul> cleaner(this);
  	return static_binop(left(),right(), std::multiplies<int>());
  }
  return this;
}

void Mul::encode() const{
  Binary::encode();
  std::cout << "# Multiplication\n"
    "  popl   %edx\n"
    "  popl   %eax\n"
    "  imull  %edx, %eax\n"
    "  pushl  %eax\n"
    ;
}
