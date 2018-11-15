/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Less.h"
#include "IntLit.h"

Less::Less(Expr *l, Expr *r)
  : Binary(l, r){
}

void Less::dump() const{
  std::cerr << " Less-than\n";
  Binary::dump();
}

Node *Less::analyze(){
  extern bool optimizing;
  Binary::analyze();
  if(!type_equal(Type::int_type()) or !types_match())
  	mismatch_error("<", Type::int_type());
  if(optimizing and is_constexpr()){
  	std::auto_ptr<Less> cleaner(this);
  	return static_binop(left(),right(), std::less<int>());
  }
  return this;
}

void Less::encode() const{
  Binary::encode();
  std::cout << "# Less-than\n"
    "  popl   %edx\n"
    "  popl   %eax\n"
    "  cmpl   %edx, %eax\n"
    "  setl   %al\n"
    "  movzbl %al, %eax\n"
    "  pushl  %eax\n"
    ;
}
