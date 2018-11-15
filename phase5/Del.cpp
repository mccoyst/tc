/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include <sstream>
#include "Del.h"

Del::Del(Expr *expr)
  : expr(expr){
  assert(expr != 0);
}

void Del::dump() const{
  expr->dump();
  std::cerr << " delete of expression of type " << *expr->type() << '\n';
}

Node *Del::analyze(){
  expr = dynamic_cast<Expr*>(expr->analyze());
  assert(expr != 0);
  if(expr->type()->is(Type::unknown()))
    error("Cannot delete expression of unknown type");
  if(expr->type_equal(Type::int_type()) or expr->type_equal(Type::null()))
    error("Cannot delete expression of type " + expr->type()->str());
  else if(expr->type()->incomplete())
    error("Type " + expr->type()->str() + " is undefined");
  return this;
}

void Del::encode() const{
  std::cout << "# delete expr\n";
  expr->encode();
  std::cout << "# delete work\n"
    "\tpushl  $" << line() << "\n"
    "\tcall   _TRUNTIME_check_null\n"
    "\tpopl   %ecx\n"
    "\tpopl   %eax\n"
    "\tpushl  %eax\n"
    "\tmovl   (%eax), %eax\n"
    "\taddl   $4, %eax\n"
    "\tmovl   (%eax), %eax\n"
    "\tcall   *%eax\n"
    "\tcall   free\n"
    "\taddl   $4, %esp\n"
    ;
}
