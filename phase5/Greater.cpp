/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Greater.h"
#include "IntLit.h"

Greater::Greater(Expr *l, Expr *r)
  : Binary(l, r){
}

void Greater::dump() const{
  std::cerr << " Greater-than\n";
  Binary::dump();
}

Node *Greater::analyze(){
  extern bool optimizing;
  Binary::analyze();
  if(!type_equal(Type::int_type()) || !types_match())
  	mismatch_error(">", Type::int_type());
  if(optimizing && is_constexpr()){
  	std::auto_ptr<Greater> cleaner(this);
  	return static_binop(left(),right(), std::greater<int>());
  }
  return this;
}

void Greater::encode() const{
  Binary::encode();
  std::cout << "# Greater-than\n"
    "  popl   %edx\n"
    "  popl   %eax\n"
    "  cmpl   %edx, %eax\n"
    "  setg   %al\n"
    "  movzbl %al, %eax\n"
    "  pushl  %eax\n"
    ;
}
