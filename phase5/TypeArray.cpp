/* Steve McCoy */

#include "Type.h"
#include "Meth.h"
#include <iostream>
#include <sstream>
#include <set>

class Array : public Type{
  Type *elem;
  unsigned int dims;
public:
  Array(Type *e, unsigned int d)
    : elem(e), dims(d){
  }

  std::string make_str() const{
    std::ostringstream os;
    os << *element();
    for(unsigned int i=0; i < dims; ++i)
      os << "[]";
    return os.str();
  }

  std::string str() const{
    std::string _ = make_str();
    return _;
  }

  bool is(const Type *t) const{
    const Array *a = dynamic_cast<const Array*>(t);
    if(!a) return false;
    return this->element()->str() == a->element()->str()
      and this->dimensions() == a->dimensions();
  }

  bool incomplete() const{
    return element()->incomplete();
  }

  void complete(bool c){
  }

  bool defined() const{
    return element()->defined();
  }

  void defined(bool d){
  }

  bool inherits_from(const Type *t) const{
    return t->is(Type::object());
  }

  bool is_array_convertible(const Type *t) const{
    const Array *a = dynamic_cast<const Array*>(t);
    return a
      and (a->dimensions() == this->dimensions())
      and this->element()->can_upcast(a->element());
  }

  bool can_upcast(const Type *t) const{
    /*
       - From any array type to type Object.
       - From any array type SC[] to any array type TC[],
         provided that SC and TC are reference types and there
         is a widening conversion from SC to TC. 
    */
    return t->is(Type::object()) or is_array_convertible(t);
  }

  // merge w/ is_array_convertible(), make it static
  bool is_array_downconvertible(const Type *t) const{
    const Array *a = dynamic_cast<const Array*>(t);
    return a
      and (a->dimensions() == this->dimensions())
      and a->element()->can_upcast(this->element());
  }

  bool can_downcast(const Type *t) const{
    return t->is(Type::object()) or is_array_downconvertible(t);
  }

  void encode() const{
    // there's just one array base type
  }

  int words() const{
    return -1;
  }

  bool field(Ident id, Type *t){
    return false;
  }

  static const Ident Length;

  int field_offset(Ident f) const{
    return (f == Length) ? 1 : -1;
  }

  void set_offsets(){
  }

  const Type *field_type(Ident f) const{
    return (f == Length) ? Type::int_type() : 0;
  }

  const Type *super() const{
    return Type::object();
  }

  void super(Type *s){
  }

  void dump() const{
    std::cerr << "Array of " << *element() << " and dimension " << dims << '\n';
  }

  void register_subclass(CDecl *c){
  }

  void define_destructor(){
  }

  bool has_destructor() const{
    return true;
  }

  std::string nearest_destructor() const{
    return "Object$.destructor";
  }

  bool define_constructor(Ctor *c){
    return false;
  }

  std::string nearest_constructor() const{
    return "ERROR array$.constructor";
  }

  Ctor *matching_constructor(const std::list<Expr*> *args) const{
    return 0;
  }

  bool has_noarg() const{
    return Type::object()->noarg();
  }

  Ctor *noarg() const{
    return Type::object()->noarg();
  }

  bool add_meth(Meth *m){
    return false;
  }

  void applicable_meths(std::list<Meth*>& l, Ident name, const std::list<Expr*> *args) const{
    return Type::object()->applicable_meths(l,name,args);
  }

  int meth_offset(const Meth *m) const{
    return Type::object()->meth_offset(m);
  }

  unsigned int dimensions() const{
    return dims;
  }

  Type *element() const{
    return elem;
  }
};

const Ident Array::Length = std::string("length");

Type *Type::array(Type *element, unsigned int dimensions){
//  static std::set<Array*, arr_less> arrays;
  Array *a, *elem;
  elem = dynamic_cast<Array*>(element);
  if(elem)
    a = new Array(elem->element(), elem->dimensions() + dimensions);
  else
    a = new Array(element, dimensions);
  return a;
//  std::pair<std::set<Array*>::iterator, bool> p = arrays.insert(a);
//  if(p.second) { std::cerr << "giving " << *a << '\n'; return a; }
//  else{ delete a; std::cerr << "-giving " << **p.first << '\n'; return *p.first; }
}

void Type::encode_array(){
  std::cout << "# array vmt\n"
    "\t.text\n"
    "\t.globl  array_vmt\n"
    "\t.data\n"
    "array_vmt:\n"
    "\t.long   Object$VMT\n"
    "\t.long   Object$.destructor\n"
    "\t.long   Object$equals$Object\n"
    ;
}
