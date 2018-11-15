/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Div.h"
#include "IntLit.h"

Div::Div(Expr *l, Expr *r)
  : Binary(l, r){
}

void Div::dump() const {
  std::cerr << " Division " << *type() << '\n';
  Binary::dump();
}

Node *Div::analyze(){
  extern bool optimizing;
  Binary::analyze();
  if(!type_equal(Type::int_type()) or !types_match())
    mismatch_error("/", Type::int_type());
  if(optimizing and is_constexpr()){
    std::auto_ptr<Div> cleaner(this);
    return static_binop(left(),right(), std::divides<int>());
  }
  return this;
}

void Div::encode() const{
  Binary::encode();
  std::cout << "# Division\n"
    "  popl   %ecx\n"
    "  cmpl   $0, %ecx\n"
    "  je     1f\n"
    "  popl   %eax\n"
    "  cltd\n"
    "  idiv   %ecx, %eax\n"
    "  pushl  %eax\n"
    "  jmp    2f\n"
    "1:\n"
    "  pushl  $" << line() << "\n"
    "  call   _TLANGUAGE_RUNTIME_ERROR_divzero\n"
    "2:\n"
    ;
}

