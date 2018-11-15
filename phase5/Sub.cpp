/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Sub.h"
#include "IntLit.h"

Sub::Sub(Expr *l, Expr *r)
  : Binary(l, r){
}

void Sub::dump() const{
  std::cerr << " Subtraction\n";
  Binary::dump();
}

/** Calls error() if subexpression is not of type int
*/
Node *Sub::analyze(){
  extern bool optimizing;
  Binary::analyze();
  if(!type_equal(Type::int_type()) or !types_match())
    mismatch_error("-", Type::int_type());
  if(optimizing and is_constexpr()){
    std::auto_ptr<Sub> cleaner(this);
    return static_binop(left(),right(), std::minus<int>());
  }
  return this;
}

void Sub::encode() const{
  Binary::encode();
  std::cout << "# Subtraction\n"
    "  popl   %edx\n"
    "  popl   %eax\n"
    "  subl   %edx, %eax\n"
    "  pushl  %eax\n"
    ;
}
