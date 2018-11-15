/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "ExprStat.h"

ExprStat::ExprStat(Expr *e)
  : expr(e){
  assert(e != 0);
}

void ExprStat::dump() const{
  std::cerr << " Expression Statement\n";
  expr->dump();
}

Node *ExprStat::analyze(){
  expr = dynamic_cast<Expr*>(expr->analyze());
  assert(expr != 0);
  return this;
}

void ExprStat::encode() const{
  std::cout << "# Expression statement\n";
  expr->encode();
  assert(expr != 0);
  std::cout <<
    "\taddl   $4, %esp\n"
    "# End expression statement\n"
    ;
}
