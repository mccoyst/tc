/*Steve McCoy
*/

#include <cstdlib>
#include <iostream>
#include "Node.h"

extern uint srcline;

Node::Node()
  : lineno(srcline){
}

Node::~Node(){
}

uint Node::line() const{
  return lineno;
}

void Node::error(const char *msg, Severity::Severity s) throw(SyntaxError){
  static const char *heads[Severity::NLevels] = {
	  "Warning for",
	  "Error at",
	  "Error at"
  };
  std::cout.flush();
  std::cerr << heads[s] << " line " << line() << ":\n " << msg << '\n';
  if(s != Severity::Warning) ++nerrors;
  if(s == Severity::Error) throw SyntaxError();
}

uint Node::nerrors = 0;

void Node::error(const std::string& msg, Severity::Severity s) throw(SyntaxError){
  error(msg.c_str(), s);
}

uint Node::errors(){
  return nerrors;
}
