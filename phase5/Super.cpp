/*Steve McCoy
*/

#include "Super.h"
#include <iostream>

Expr *super_expr(){
  static class _ : public Expr{
  public:
    void dump() const{
      std::cerr << " super\n";
    }
    Node *analyze(){
      const Type *cur = Type::current();
      if(!cur) error("'super' outside of a class declaration");
      type(cur->super());
      return this;
    }
    void encode() const{
      std::cout << "# Super\n"
        "\tmovl   8(%ebp), %eax\n"
        "\tpushl  %eax\n"
        ;
    }
  } se;
  return &se;
}
