/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Neg.h"
#include "IntLit.h"

Neg::Neg(Expr *sub)
  : Unary(sub){
  IntLit *n =  dynamic_cast<IntLit*>(sub);
  if(n) n->is_negated = true;
}

void Neg::dump() const{
  std::cerr << " Negation\n";
  Unary::dump();
}

Node *Neg::analyze(){
  extern bool optimizing;
  Unary::analyze();
  if(!type_equal(Type::int_type()))
  	mismatch_error("-", Type::int_type());
  if(optimizing && is_constexpr()){
  	std::auto_ptr<Neg> cleaner(this);
  	return static_unop(subexpr(), std::negate<int>());
  }
  return this;
}

void Neg::encode() const{
  Unary::encode();
  std::cout << "# Math negation\n"
    "\tpopl   %eax\n"
    "\tnegl   %eax\n"
    "\tpushl  %eax\n"
    ;
}
