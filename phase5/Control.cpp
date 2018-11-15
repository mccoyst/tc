/*Steve McCoy
*/

#include <cassert>
#include <typeinfo>
#include "Control.h"
#include "IntLit.h"

Control::Control(Expr *c)
	: cond(c){
}

void Control::mismatch_error(const std::string& nm, const Type *ex){
	error("'" + nm + "' expects condition of type " + ex->str()
	  + " and got " + cond->type()->str());
}

bool Control::is_constexpr() const{
	return typeid(*cond) == typeid(IntLit);
}

Node *Control::analyze(){
	cond = dynamic_cast<Expr*>(cond->analyze());
	return this;
}
