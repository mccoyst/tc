/*Steve McCoy
*/

#include <iostream>
#include "NullLit.h"

Expr *null_lit(){
  static class _ : public Expr{
  public:
    _(){
      type(Type::null());
    }
    void dump() const{
      std::cerr << " null literal\n";
    }
    Node *analyze(){
      return this;
    }
    void encode() const{
    std::cout << "# null\n"
      "  pushl  $0\n"
      ;
    }
  } nl;
  return &nl;
}
