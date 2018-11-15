/*Steve McCoy
*/

#include "Dest.h"
#include "Function.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <cassert>

Dest::Dest(Ident id, Block *db)
  : Function(0, id, new std::list<Param*>(), db){
}

void Dest::dump() const{
  std::cerr << " Destructor declaration\n~";
  Function::dump(true);
  std::cerr << " End destructor declaration\n";
}

Node *Dest::analyze(){
  enclosing = Type::current();
  if(!enclosing)
    error("Cannot define destructor '~" + name.str() + "' outside of a class declaration");
  if(Type::user(name)->is(enclosing))
    analyze_body();
  else
    error("Cannot define destructor '~" + name.str() + "' in class " + enclosing->str());
  return this;
}

void Dest::encode() const{
  std::cout << "# Destructor for " << name << "\n"
    "\t.align 4\n"
    "\t.globl " << name << "$.destructor\n"
    << name << "$.destructor:\n"
    "\tpushl %ebp\n"
    "\tmovl  %esp, %ebp\n"
    ;
  encode_body();
  std::cout <<
    "\tcall  " << Type::user(name)->super()->nearest_destructor() << "$.destructor\n"
    "\tmovl  $0, %eax\n"
    << name << "$.destructor.exit:\n"
    "\tpopl  %ebp\n"
    "\tret\n"
    ;
}

void Dest::add_to(Type *t){
  if(t->has_destructor())
    error("Class " + t->str() + " cannot have two destructors");
  else
    t->define_destructor();
}
