/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Return.h"
#include "Meth.h"

Return::Return(Expr *v)
  : Control(v), in_main(true){
}

void Return::dump() const{
  if(cond){
    std::cerr << " Return value of type " << *cond->type() << "\n"
	    " Return expression:\n";
    cond->dump();
  }else
    std::cerr << " Return with no expression\n";
}

Node *Return::analyze(){
  if(cond){
   cond = dynamic_cast<Expr*>(cond->analyze());
   assert(cond != 0);
  }
  if(Function::current()){ // inside a method
    if(!Function::current()->rtype()){ // inside a constructor/destructor
      if(cond)
        error("Cannot return a value from a constructor or destructor");
      in_main = false;
    }else{
      if(!cond)
        error("Must return a value from a method");
      if(!cond->type()->can_upcast(Meth::current()->rtype()))
        mismatch_error("return", Meth::current()->rtype());
      in_main = false;
    }
  }else{ // inside main()
    if(!cond)
      error("Must return a value from main");
    else if(!cond->type_equal(Type::int_type()))
	    mismatch_error("return", Type::int_type());
  }
  return this;
}

void Return::encode() const{
  if(cond) cond->encode();
  if(in_main)
    std::cout << "# main return\n"
	    "\tpopl   %eax\n"
	    "\tjmp    .main_exit\n"
	    ;
  else{
    std::cout << "# method return\n";
    if(cond) std::cout << "\tpopl   %eax\n";
    std::cout << "\tjmp    " << Function::current()->munge() << ".exit\n";
  }
}
