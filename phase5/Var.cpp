/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Var.h"
#include "Deref.h"
#include "SymTab.h"
#include "Function.h"

Var::Var(Ident name)
  : id(name){
}

/** Calls error(), explaining that @a name is undefined.
*/
void Var::undef_error(Ident name){
  error("'" + name.str() + "' is undefined");
}

Ident Var::name() const{
  return id;
}

void Var::type(const Type *t){
  Expr::type(t);
}

void Var::dump() const{
  std::cerr << " Variable (" << id;
  if(symbol_table().count(id) > 0){ // search in Type::current first
	  VarInfo vi = symbol_table()[id];
	  std::cerr << ", " << vi.type->str() << ",@" << vi.decloc;
  }
  std::cerr << ")\n";
}

Node *Var::analyze(){
  const Type *t = 0;
  if(Function::current())
    t = Function::current()->param_type(id);
  if(!t) t = (Type::current())
    ? Type::current()->field_type(id)
    : symbol_table()[id].type
    ;
  if(!t) undef_error(id);
  type(t);
  return new Deref(this);
}

void Var::encode() const{
  if(Function::current() and Function::current()->param_type(id)) std::cout <<
    "# Parameter variable (" << id << ")\n"
    "\tleal   " << (4*Function::current()->param_pos(id)+12) << "(%ebp), %eax\n"
    "\tpushl  %eax\n"
    ;
  else if(Type::current()) std::cout <<
    "# Instance variable (" << id << " from " << Type::current()->str() << ")\n"
    "\tmovl   8(%ebp), %eax\n"
    "\taddl   $" << (4*Type::current()->field_offset(id)) << ", %eax\n"
    "\tpushl  %eax\n"
    ;
  else std::cout <<
    "# Main variable (" << id << ")\n"
    "\tpushl  $" << id << '\n'
    ;
}
