/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Not.h"
#include "IntLit.h"

Not::Not(Expr *sub)
  : Unary(sub){
}

void Not::dump() const{
  std::cerr << " Not-ation\n";
  Unary::dump();
}

Node *Not::analyze(){
  extern bool optimizing;
  Unary::analyze();
  if(!type_equal(Type::int_type()))
  	mismatch_error("!", Type::int_type());
  if(optimizing && is_constexpr()){
  	std::auto_ptr<Not> cleaner(this);
  	return static_unop(subexpr(), std::logical_not<int>());
  }
  return this;
}

void Not::encode() const{
  Unary::encode();
  std::cout << "# Logical negation\n"
    "  pushl  $0\n"
    "  popl   %eax\n"
    "  popl   %edx\n"
    "  cmpl   %eax, %edx\n"
    "  sete   %al\n"
    "  movzbl %al, %eax\n"
    "  pushl  %eax\n"
    ;
}
