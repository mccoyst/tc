/*Steve McCoy
*/

#include "This.h"
#include <iostream>

Expr *this_expr(){
  static class _ : public Expr{
  public:
    void dump() const{
      std::cerr << " this\n";
    }
    Node *analyze(){
      const Type *cur = Type::current();
      if(!cur) error("'this' outside of a class declaration");
      type(cur);
      return this;
    }
    void encode() const{
      std::cout << "# this\n"
        "\tmovl   8(%ebp), %eax\n"
        "\tpushl  %eax\n"
        ;
    }
  } te;
  return &te;
}
