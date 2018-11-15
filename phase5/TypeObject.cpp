/* Steve McCoy */

#include "Type.h"
#include "Ctor.h"
#include "Dest.h"
#include "Meth.h"
#include <iostream>

Type *Type::object(){
  static struct _ : public Type{
    _() {}
    std::string str() const{ return "Object"; }
    bool is(const Type *t) const{ return this == t; }
    bool incomplete() const{ return false; }
    bool defined() const{ return true; }
    bool inherits_from(const Type *t) const{ return false; }
    bool can_upcast(const Type *t) const{ return is(t) or t->is(Type::null()); }
    bool can_downcast(const Type *t) const{ return !is(t) and dynamic_cast<const Type*>(t) != 0; }
    int words() const{ return 0; }
    void encode() const{
      std::cout << "# class Object:\n"
        "\t.data\n"
        "Object$VMT:\n"
        "\t.long   0\n"
        "\t.long   Object$.destructor\n"
        "\t.long   Object$equals$Object\n"
        "\t.text\n"
        "\t.align  4\n"
        "\t.globl  Object$.destructor\n"
        "Object$.destructor:\n"
        "\tpushl   %ebp\n"
        "\tmovl    %esp, %ebp\n"
        "Object$.destructor.exit:\n"
        "\tpopl    %ebp\n"
        "\tret\n"
        "Object$Object:\n"
        "\tpushl   %ebp\n"
        "\tmovl    %esp, %ebp\n"
        "Object$Object.exit:\n"
        "\tpopl    %ebp\n"
        "\tret\n"
        ;
      Type::Make_current mc(this);
      Meth::object_equals()->encode();
    }
    int field_offset(Ident f) const{ return -1; }
    void set_offsets() {}
    const Type *field_type(Ident f) const{ return 0; }
    const Type *super() const{ return 0; }
    void complete(bool c) {}
    void defined(bool c) {}
    void super(Type *r) {}
    bool field(Ident id, Type *t) { return true; }
    void register_subclass(CDecl *c) {}
    void dump() const{
      std::cerr <<
        "class Object{\n"
        "Object();\n"
        "~Object();\n"
        "int equals(Object);\n"
        "}\n"
        ;
    }
    void define_destructor(){}
    bool has_destructor() const{ return true; }
    std::string nearest_destructor() const{
      return "Object";
    }
    bool define_constructor(Ctor *c){
      return false;
    }
    std::string nearest_constructor() const{
      return "Object";
    }
    Ctor *matching_constructor(const std::list<Expr*> *args) const{
      return (args->size() == 0) ? Ctor::object() : 0;
    }
    bool add_meth(Meth *m){
      return false;
    }
    bool has_noarg() const{
      return true;
    }
    Ctor *noarg() const{
      return Ctor::object();
    }
    void applicable_ctors(std::list<Ctor*>& l, const std::list<Expr*> *args) const{
      if(args->size() == 0)
        l.push_back(Ctor::object());
    }
    void applicable_meths(std::list<Meth*>& l, Ident name, const std::list<Expr*> *args) const{
      if(name.str() == "equals" and args->size() == 1
      and args->front()->type()->can_upcast(Type::object()))
        l.push_back(Meth::object_equals());
    }
    int meth_offset(const Meth *m) const{
      return m->sig_equals(Meth::object_equals()) ? 2 : -1;
    }
  } ot;
  return &ot;
}
