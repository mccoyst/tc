/*Steve McCoy
*/

#include "Ctor.h"
#include "CInvoke.h"
#include <cassert>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <set>

extern CInvoke *explicit_superctor;
extern CInvoke *explicit_thisctor;

Ctor::Ctor(Type *n, std::list<Param*> *p, Block *b)
  : Function(0, n->str(), p, b), implicit_superctor(true){
  assert(params != 0 and body != 0);
}

void Ctor::dump() const{
  std::cerr << " Constructor\n";
  Function::dump(true);
  std::cerr << " End constructor\n";
}

Node *Ctor::analyze(){
  enclosing = Type::current();
  if(!enclosing)
    error("Cannot declare a constructor for type "+name.str()+" outside of a class declaration");
  if(Type::user(name)->is(enclosing)){
    std::for_each(params->begin(),params->end(),std::mem_fun(&Param::analyze));
    std::set<Param*,param_less> copy;
    copy.insert(params->begin(),params->end());
    if(copy.size() != params->size())
      error("Duplicate parameter name");

    Function::Make_current mc(this);
    body = dynamic_cast<Block*>(body->analyze());

    // check for CInvoke & recurse & no noarg super
    if(explicit_superctor){
      implicit_superctor = false;
      explicit_superctor = 0;
    }else if(explicit_thisctor){
      implicit_superctor = false;
      if(explicit_thisctor->calls(this)){
        explicit_thisctor = 0;
        error("Recursive constructor call");
      }
      explicit_thisctor = 0;
    }else if(Type::user(name)->super()){
      if(!Type::user(name)->super()->has_noarg()){
        error("No implicit constructor available in immediate superclass");
      }
    }
  }else
    error("Cannot declare a constructor for type "+name.str()+" inside of class "+enclosing->str());
  return this;
}
void Ctor::encode() const{
  std::cout << "# constructor for " << name << "\n"
    "\t.align  4\n"
    "\t.globl  " << munge() << "\n"
    << munge() << ":\n"
    "\tpushl   %ebp\n"
    "\tmovl    %esp, %ebp\n"
    ;
  if(implicit_superctor)
    std::cout << "# implicit ctor\n"
      "\tmovl   8(%ebp), %eax\n"
      "\tpushl  %eax\n"
      "\tcall   " << Type::current()->nearest_constructor() << "\n"
      "\tpopl   %eax\n"
      ;
  Function::Make_current mc(this);
  body->encode();
  std::cout <<
    "\tmovl    $0, %eax\n"
    << munge() << ".exit:\n"
    "\tpopl    %ebp\n"
    "\tret\n"
    ;
}

void Ctor::add_to(Type *t){
  if(!t->define_constructor(this))
    error("A constructor with the same signature is already defined in class "
    +t->str());
}


Ctor *Ctor::object(){
  static Ctor _(Type::object(), new std::list<Param*>(), new Block(new std::list<Stat*>()));
  static bool once = true;
  if(once){
    Type::Make_current mc(Type::object());
    _.analyze();
    once = false;
  }
  return &_;
}
