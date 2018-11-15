/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "tc.h"
#include "IntLit.h"

/** Constructs an integer literal with value \a n and T type int
*/
IntLit::IntLit(unsigned int n)
  : val(n), is_negated(false){
  type(Type::int_type());
}

/** Prints the value of the literal to standard error.
*/
void IntLit::dump() const{
  std::cerr << " Integer literal (" << value() << ")\n";
}

/** The type is always known, so this just returns the current object.
*/
Node *IntLit::analyze(){
  if(!is_negated and val == LargestInt)
    error("2147483648 may appear only as the operand of the unary negation operator \"-\"");
  return this;
}

/** Pushes the literal onto the stack.
*/
void IntLit::encode() const{
  std::cout << "# Integer literal\n"
    "\tpushl  $" << value() << "\n"
    ;
}
