/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include <memory>
#include "While.h"
#include "IntLit.h"
#include "EmptyStat.h"

While::While(Expr *c, Stat *s)
  : Control(c), stat(s){
  assert(stat != 0);
}

While::~While(){
  delete cond;	// leaky, do something about it
}

void While::dump() const{
  std::cerr << " While Statement\n"
	  " Condition:\n";
  cond->dump();
  std::cerr << " Repeater:\n";
  stat->dump();
}

Node *While::analyze(){
  extern bool warning, optimizing;
  Control::analyze();
  if(!cond->type_equal(Type::int_type()))
	  mismatch_error("while", Type::int_type());
  if(warning && is_constexpr()){
	  IntLit *c = dynamic_cast<IntLit*>(cond);
	  if(c->value() == 0)
		  error("while loop statement will never be executed", Severity::Warning);
	  else
		  error("while loop is infinite", Severity::Warning);
  }
  if(optimizing && is_constexpr()){
	  std::auto_ptr<While> cleaner(this);
	  IntLit *c = dynamic_cast<IntLit*>(cond);
	  if(c->value() == 0){
		  delete stat;
		  return empty_stat();
	  }
  }
  While::push();
  entry = While::label1();
  exit = While::label2();
  stat = dynamic_cast<Stat*>(stat->analyze());
  assert(stat != 0);
  While::pop();
  return this;
}

unsigned int While::depth = 0;
std::stack<unsigned int> While::stack;
const char *While::LblPrefix = ".W";	// '.' can appear in labels but not T identifiers

void While::push(){
  While::stack.push(While::depth);
  While::depth += 2;	// nest by even numbers to avoid inner labels that equal label2
}

void While::pop(){
  While::stack.pop();
}

unsigned int While::label1(){
  return While::stack.top();
}

unsigned int While::label2(){
  return While::stack.top()+1;
}

bool While::inside(){
  return !While::stack.empty();
}

void While::encode() const{
  std::cout << "# while loop\n" <<
    While::LblPrefix << entry << ":\n";
  cond->encode();
  std::cout <<
	  "# while test:\n"
	  "  popl   %eax\n"
	  "  cmpl   $0, %eax\n"
	  "  je     " << While::LblPrefix << exit << "\n"
	  ;
  stat->encode();
  std::cout <<
	  "# while repeat\n"
	  "  jmp    " << While::LblPrefix << entry << "\n" <<
	  While::LblPrefix << exit << ":\n"
	  ;
}
