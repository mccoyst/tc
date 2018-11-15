/* Steve McCoy */

#include "AAccess.h"
#include <iostream>
#include "Deref.h"

AAccess::AAccess(Expr *l, Expr *r)
  : Binary(l,r){
}

void AAccess::dump() const{
  std::cerr << " Array access\n";
  Binary::dump();
  std::cerr << " End array access\n";
}

Node *AAccess::analyze(){
  Binary::analyze();
  if(left()->type()->dimensions() == 0)
    error("Can't index into an instance of non-array type '"+left()->type()->str()+"'");
  if(!right()->type()->is(Type::int_type()))
    error("Can't index into an array with a non-int expression");
  if(left()->type()->dimensions() == 1)
    type(left()->type()->element());
  else
    type(Type::array(left()->type()->element(), left()->type()->dimensions()-1));
  return new Deref(this);
}

void AAccess::encode() const{
  std::cout << "# array access\n";
  Binary::encode();
  std::cout <<
    "#  line number:\n"
    "\tpushl   $" << line() << "\n"
    "#  real work:\n"
    "\tcall    _TRUNTIME_array_access\n"
    "\taddl    $12, %esp\n"
    "\tpushl   %eax\n"
    ;
}
