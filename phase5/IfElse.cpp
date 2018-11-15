/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include <memory>
#include "IfElse.h"
#include "IntLit.h"

IfElse::IfElse(Expr *c, Stat *i, Stat *e)
  : Control(c), ifstat(i), elstat(e){
  assert(ifstat != 0 && elstat != 0);
}

IfElse::~IfElse(){
  delete cond;	// leaky, do something about it
}

void IfElse::dump() const{
  std::cerr << " If-Else Statement\n"
    " Condition:\n";
  cond->dump();
  std::cerr << " If Branch:\n";
  ifstat->dump();
  std::cerr << " Else Branch:\n";
  elstat->dump();
}

Node *IfElse::analyze(){
  extern bool optimizing;
  Control::analyze();
  if(!cond->type_equal(Type::int_type()))
    mismatch_error("if-else", Type::int_type());
  if(optimizing && is_constexpr()){
    std::auto_ptr<IfElse> cleaner(this);
    IntLit *c = dynamic_cast<IntLit*>(cond);
    if(c->value() != 0){
      delete elstat;
      return ifstat->analyze();
    }else{
      delete ifstat;
      return elstat->analyze();
    }
  }
  ifstat = dynamic_cast<Stat*>(ifstat->analyze());
  elstat = dynamic_cast<Stat*>(elstat->analyze());
  assert(ifstat != 0 && elstat != 0);
  return this;
}

unsigned int IfElse::depth = 0;
std::stack<unsigned int> IfElse::stack;
const char *IfElse::LblPrefix = ".I";	// '.' can appear in labels but not T identifiers

void IfElse::push(){
  IfElse::stack.push(depth);
  depth += 2;	// nest by even numbers to avoid inner labels that equal label2
}

void IfElse::pop(){
  IfElse::stack.pop();
}

unsigned int IfElse::label1(){
  return IfElse::stack.top();
}

unsigned int IfElse::label2(){
  return IfElse::stack.top()+1;
}

void IfElse::encode() const{
  IfElse::push();

  std::cout << "# if-else condition:\n";
  cond->encode();
  std::cout << "# if-else test:\n"
    "  popl   %eax\n"
    "  cmpl   $0, %eax\n"
    "  je     " << IfElse::LblPrefix << IfElse::label1() << "\n"
    "# ifstat:\n"
    ;
  ifstat->encode();
  std::cout <<
    "  jmp    " << IfElse::LblPrefix << IfElse::label2() << '\n' << 
    IfElse::LblPrefix << IfElse::label1() << ":\n"
    "# elstat:\n"
    ;
  elstat->encode();
  std::cout << IfElse::LblPrefix << IfElse::label2() << ":\n";
  IfElse::pop();
}
