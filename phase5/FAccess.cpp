/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include <sstream>
#include "FAccess.h"
#include "Deref.h"

FAccess::FAccess(Expr *left, Ident field)
  : left(left), field(field){
  assert(left != 0);
}

void FAccess::dump() const{
  left->dump();
  std::cerr << " Field access of " << field << " from " << *left->type() << '\n';
}

Node *FAccess::analyze(){
  left = dynamic_cast<Expr*>(left->analyze());
  assert(left != 0);
  if(left->type()->is(Type::int_type()))
    error("Operator '.' expects left operand of reference type and got " + left->type()->str());
  else if(left->type()->incomplete())
    error("Cannot access fields from incomplete type " + left->type()->str());
  else if(left->type()->is(Type::null()))
    error("Operator '.' cannot be applied to the null literal");
  else if(left->type()->field_offset(field) == -1)
    error("'" + field.str() + "' isn't a member of class " + left->type()->str());

  else
    type(left->type()->field_type(field));
  return new Deref(this);
}

void FAccess::encode() const{
  left->encode();
  std::cout << "# field access of " << field << " from " << left->type()->str() << "\n"
    "\tpushl $" << line() << "\n"
    "\tcall  _TRUNTIME_check_null\n"
    "\taddl  $4, %esp\n"
    "\tpopl  %eax\n"
    "\taddl  $" << (left->type()->field_offset(field)*4) << ", %eax\n"
    "\tpushl %eax\n"
    ;
}
