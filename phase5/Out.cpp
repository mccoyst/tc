/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Out.h"

Out::Out(Expr *e)
  : sub(e){
  assert(e != 0);
}

Out::~Out(){
  delete sub;
}

void Out::dump() const{
  std::cerr << " Output statement\n";
  sub->dump();
}

Node *Out::analyze(){
  sub = dynamic_cast<Expr*>(sub->analyze());
  assert(sub != 0);
  if(!sub->type_equal(Type::int_type()))
    error("'out' expects an integer expression operand, got " + sub->type()->str());
  return this;
}

void Out::encode() const{
  sub->encode();
  std::cout <<
    "# Output statement\n"
    "	call	_TLANGUAGE_RUNTIME_outint\n"
    "	addl	$4, %esp\n"
    ;
}
