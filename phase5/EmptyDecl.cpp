/*Steve McCoy
*/

#include <iostream>
#include "EmptyDecl.h"

CBDecl *empty_decl(){
  static class _ : public CBDecl{
  public:
    void dump() const{ std::cerr << " Empty member declaration\n"; }
    Node *analyze(){ return this; }
    void encode() const{}
    void add_to(Type *t){}
    bool complete() const{ return true; }
    std::string str() const{ return "''"; }
  } ed;
  return &ed;
}
