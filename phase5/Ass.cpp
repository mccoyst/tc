/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Ass.h"
#include "Deref.h"
#include "FAccess.h"
#include "AAccess.h"

Ass::Ass(Expr *l, Expr *r)
  : Binary(l, r), inserting(false){
}

void Ass::dump() const{
  std::cerr << " Assignment " << type()->str() << '\n';
  Binary::dump();
}

inline bool is_array_length(const Expr *e){
  const FAccess *fa = dynamic_cast<const FAccess*>(e);
  if(!fa) return false;
  return fa->expr()->type()->dimensions() > 0 and fa->field_name() == std::string("length");
}

inline bool is_ref_array_access(const Expr *e){
  const AAccess *a = dynamic_cast<const AAccess*>(e);
  return a and !a->left()->type()->is(Type::int_type());
}

Node *Ass::analyze(){
  Deref *derf = dynamic_cast<Deref*>(left()->analyze());
  assert(derf != 0);
  left(derf->subexpr());
  derf->deref();
  delete derf;
  assert(left() != 0);
  right(dynamic_cast<Expr*>(right()->analyze()));
  assert(right() != 0);
  if(is_array_length(left()))
    error("Cannot modify an array's length field");
  if(!types_match())
    mismatch_error("=", left()->type());
  if(is_ref_array_access(left()))
    inserting = true;
  type(left()->type());
  return this;
}

void Ass::encode() const{
  //Binary::encode();
  if(inserting){
    AAccess *a = dynamic_cast<AAccess*>(left());
    std::cout << "# checked array element assignment\n"
      "#  target array:\n";
    a->left()->encode();
    std::cout << "#  index into target:\n";
    a->right()->encode();
    std::cout << "#  thing to insert:\n";
    right()->encode();
    std::cout << "#  line:\n"
      "\tpushl $" << line() << "\n"
      "\tcall  _TRUNTIME_array_insert\n"
      "\taddl  $16, %esp\n"
      "\tpushl %eax\n"
      ;
  }else{
    Binary::encode();
    std::cout << "# Assignment\n"
      "\tpopl  %eax\n"
      "\tpopl  %edx\n"
      "\tmovl  %eax, (%edx)\n"
      "\tpushl %eax\n"
      ;
  }
}
