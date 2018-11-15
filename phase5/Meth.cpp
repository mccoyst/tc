/*Steve McCoy*/

#include "Meth.h"
#include "Return.h"
#include "Equals.h"
#include "Var.h"
#include "This.h"
#include <cassert>
#include <iostream>

Meth::Meth(Type *t, Ident i, std::list<Param*> *p, Block *b)
  : Function(t,i,p,b){
}

void Meth::dump() const{
  std::cerr << " Declaration of method:\n";
  Function::dump(true);
  std::cerr << " End method '" << name << "' declaration\n";
}

Node *Meth::analyze(){
  if(!Type::current()) error("Can't define method '"+name.str()+"' outside of a class");
  enclosing = Type::current();
  analyze_params();

  Ident id("int");
  if(duplicated_param_name(id)) error("Duplicated parameter '"+id.str()+"'");

  analyze_body();
  return this;
}

void Meth::encode() const{
  std::cout << "# method " << Type::current()->str() << '.' << name << " body\n"
    "\t.align  4\n"
    "\t.globl  " << munge() << "\n"
    << munge() << ":\n"
    "\tpushl   %ebp\n"
    "\tmovl    %esp, %ebp\n"
    ;
  encode_body();
  std::cout <<
    "\tmovl    $0, %eax\n"
    << munge() << ".exit:\n"
    "\tpopl    %ebp\n"
    "\tret\n"
    ;
}

void Meth::add_to(Type *t){
  if(!t->add_meth(this))
    error("A method with the same signature is already defined in class "+t->str());
}

inline Meth *make_object_equals(){
  Meth *oe;
  Ident that = std::string("that"),
    equals = std::string("equals");
  std::list<Param*> *p = new std::list<Param*>();
  p->push_back(new Param(Type::object(), that, 0));

  std::list<Stat*> *s = new std::list<Stat*>();
  s->push_back(new Return(new Equals(this_expr(), new Var(that))));

  oe = new Meth(Type::int_type(), equals, p, new Block(s));
  Type::Make_current mc(Type::object());
  oe->analyze();
  return oe;
}

Meth *Meth::object_equals(){
  static Meth *oe = make_object_equals();
  return oe;
}
