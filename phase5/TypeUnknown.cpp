/* Steve McCoy */

#include "Type.h"
#include <iostream>

Type *Type::unknown(){
	static struct _ : public Type{
		_() {}
    std::string str() const{ return "[unknown]"; }
		bool is(const Type *t) const{ return this == t; }
    bool incomplete() const{ return false; }
    bool defined() const{ return false; }
    bool inherits_from(const Type *t) const{ return false; }
    bool can_upcast(const Type *t) const{ return false; }
    bool can_downcast(const Type *t) const{ return false; }
    int words() const{ return 0; }
    void encode() const{
      std::cout << "ERROR unknown type's \"VMT\"\n";
    }
    int field_offset(Ident i) const{ return -1; }
    void set_offsets() {}
    const Type *field_type(Ident f) const{ return 0; }
    const Type *super() const{ return 0; }
    void complete(bool c) {}
    void defined(bool d) {}
    void super(Type *r) {}
    bool field(Ident id, Type *t) { return true; }
    void register_subclass(CDecl *c) {}
    void dump() const{}
    void define_destructor(){}
    bool has_destructor() const{ return false; }
    std::string nearest_destructor() const{
      return "Error$";
    }
    bool define_constructor(Ctor *c){ return false; }
    std::string nearest_constructor() const{
      return "Error$";
    }
    Ctor *matching_constructor(const std::list<Expr*> *args) const{
      return 0;
    }
    bool add_meth(Meth *m){
      return false;
    }
    bool has_noarg() const{
      return false;
    }
    Ctor *noarg() const{
      return 0;
    }
    void applicable_meths(std::list<Meth*>& l, Ident name, const std::list<Expr*> *args) const{
    }
    int meth_offset(const Meth *m) const{
      return -1;
    }
	} ut;
	return &ut;
}
