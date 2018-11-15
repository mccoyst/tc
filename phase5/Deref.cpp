/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Deref.h"

Deref::Deref(Expr *sub)
	: Unary(sub){
	type(sub->type());
}

void Deref::dump() const{
	std::cerr << " Deref " << *type() << '\n';
	Unary::dump();
}

Node *Deref::analyze(){
	assert(false && "derefs should only be added in analysis");
	return this;
}

void Deref::encode() const{
	Unary::encode();
	std::cout << "# Deref\n"
    "\tpopl  %eax\n"
    "\tmovl  (%eax), %eax\n"
    "\tpushl %eax\n"
		;
}

void Deref::deref(){
  subexpr(0);
}
