/* Steve McCoy */

#include "Type.h"
#include "SymTab.h"
#include "Meth.h"
#include "Ctor.h"
#include "Dest.h"
#include "CDecl.h"
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>


struct FieldInfo{
  Type *type;
  int offset;
  FieldInfo() : type(0), offset(0) {}
  FieldInfo(Type *t, int o) : type(t), offset(o) {}
};

Type *Type::user(Ident name){
  class _ : public Type{
    Ident id;
    bool comp, defd;
    Type *base;
    std::map<Ident,FieldInfo> fields;
    std::list<CDecl*> subs;
    bool custom_d;
    std::list<Ctor*> ctors;
    std::vector<Meth*> vmt;
  public:
    _(Ident i) : id(i), comp(false), defd(false), custom_d(false){
    }

    std::string str() const{
      return id.str();
    }

    bool is(const Type *t) const{
      const _ *u = dynamic_cast<const _*>(t);
      return u ? (id == u->id) : false;
    }

    bool incomplete() const{
      return !comp;
    }

    bool defined() const{
      return defd;
    }

    bool inherits_from(const Type *t) const{
      const Type *s = base;
      if(!t || !s) return false;
      while(s){
        if(s->is(t)) return true;
        s = s->super();
      }
      return false;
    }

    bool can_upcast(const Type *t) const{
      return is(t) or t->is(Type::null()) or inherits_from(t);
    }

    bool can_downcast(const Type *t) const{
      return t->inherits_from(this);
    }

    int words() const{
      return super()->words() + fields.size();
    }

    std::string nearest_destructor() const{
      if(has_destructor()) return str();
      const Type *s = base;
      while(s){
        if(s->has_destructor()) return s->str();
        s = s->super();
      }
      return "Error$";
    }

    void encode() const{
      Type::Make_current mc(this);
      std::cout << "# class " << str() << "\n"
        "\t.data\n"
        << str() << "$VMT:\n"
        "\t.long " << super()->str() << "$VMT\n"
        ;
      std::cout << "\t.long  " << nearest_destructor() << "$.destructor\n";
      std::vector<Meth*>::const_iterator m = vmt.begin();
      for(; m != vmt.end(); ++m)
        std::cout << "\t.long  " << (*m)->munge() << " # " << (4*meth_offset(*m)) << '\n';
      std::cout << "\t.text\n";
    }

    int field_offset(Ident f) const{
      std::map<Ident,FieldInfo>::const_iterator i = fields.find(f);
      if(i == fields.end()) return super()->field_offset(f);
      else return i->second.offset;
    }

    void set_offsets(){
      base->set_offsets();  // gotta do this every time to be sure
      int o = super()->words()+1;
      for(std::map<Ident,FieldInfo>::iterator i = fields.begin(); i != fields.end(); ++i){
        i->second.offset = o;
        ++o;
      }
    }

    const Type *field_type(Ident f) const{
      std::map<Ident,FieldInfo>::const_iterator i = fields.find(f);
      if(i == fields.end()) return super()->field_type(f);
      else return i->second.type;
    }

    const Type *super() const{
      return base;
    }

    void complete(bool c){
      comp = c;
      if(comp) std::for_each(subs.begin(),subs.end(), std::mem_fun(&CDecl::update));
    }

    void defined(bool d){
      defd = d;
    }

    void super(Type *s){
      base = s;
      if(base->is(Type::object())){
        if(meth_offset(Meth::object_equals()) == -1)
          vmt.insert(vmt.begin(), Meth::object_equals());
      }else{
        if(!base->incomplete()){
        _ *t = dynamic_cast<_*>(base);
        if(!t){ return; }// error mode
        // add only the super's meths that aren't overridden - start from scratch, basically
        std::vector<Meth*> old_vmt = vmt;
        vmt = t->vmt;
        std::vector<Meth*>::iterator m = old_vmt.begin();
        for(; m != old_vmt.end(); ++m)
          add_meth(*m);
        }
      }
    }

    bool field(Ident id, Type *t){
      std::map<Ident,FieldInfo>::const_iterator i = fields.find(id);
      if(i == fields.end()){
        fields[id] = FieldInfo(t,-1);
        return true;
      }else return false;
    }

    void register_subclass(CDecl *c){
      subs.push_back(c);
    }

    static inline void print_field(const std::pair<Ident,FieldInfo>& f){
      std::cerr << f.second.type->str()<<' '<<f.first<<"; // @"<<f.second.offset<<"\n";
    }

    void dump() const{
      std::cerr << "class " << str();
      if(super()) std::cerr << " extends " << super()->str();
      else std::cerr << " is incomplete ";
      std::cerr << "{\n";
      std::for_each(fields.begin(),fields.end(),print_field);
      std::for_each(ctors.begin(),ctors.end(),
        std::bind1st(std::mem_fun(&Function::dump),false));
      if(has_destructor())
        std::cerr << "  ~" << str() << "()\n";
      std::for_each(vmt.begin(),vmt.end(),
        std::bind1st(std::mem_fun(&Function::dump),false));
      std::cerr << "}\n";
    }

    void define_destructor(){
      custom_d = true;
    }

    bool has_destructor() const{
      return custom_d;
    }

    bool define_constructor(Ctor *c){
/*NEED TO MOVE SIG_EQUALS TO METHOD INTERFACE     I didddd???????????????????
        list<Meth*>::const_iterator i = find_if(ctors.begin(),ctors.end(),
        bind1st(mem_fun(&Meth::sig_equals),m));
      if(i == ctors.end()){
*/
        ctors.push_back(c);
        return true;
/*      }else
        return false;
*/
    }

    std::string nearest_constructor() const{
      if(super()->has_noarg()) return super()->noarg()->munge();
      else return "ERROR no inherited default";
    }

    Ctor *matching_constructor(const std::list<Expr*> *args) const{
      std::list<Ctor*>::const_iterator c;
      for(c = ctors.begin(); c != ctors.end(); ++c)
        if((*c)->matches(args)) return *c;
      if(args->size() == 0 and super()->has_noarg())
        return super()->noarg();
      else
        return 0;
    }

    void applicable_ctors(std::list<Ctor*>& l, const std::list<Expr*> *args) const{
      std::list<Ctor*>::const_iterator c = ctors.begin();
      while(c != ctors.end()){
        if((*c)->matches(args))
          l.push_back(*c);
        ++c;
      }
    }

    bool has_noarg() const{
      std::list<Ctor*>::const_iterator c;
      for(c = ctors.begin(); c != ctors.end(); ++c)
        if((*c)->nparams() == 0) return true;
      return ctors.size() == 0 and super()->has_noarg();
    }

    Ctor *noarg() const{
      std::list<Ctor*>::const_iterator c;
      for(c = ctors.begin(); c != ctors.end(); ++c)
        if((*c)->nparams() == 0) return *c;
      return (super()->has_noarg()) ? super()->noarg() : 0;
    }

    bool add_meth(Meth *m){
      int i = super()->meth_offset(m);
      if(i == -1){  // not a superclass method
        i = meth_offset(m);
        if(i == -1){  // not already defined
          vmt.push_back(m);
          return true;
        }else
          return false;
      }else{  // override superclass method
     //   super(base); // ugh
        if(!vmt[i-2]->rtype()->is(m->rtype())){
          extern int lexerrs;
          ++lexerrs;
          std::cerr << "Error at line " << m->line()
            << ":\n Function and inherited method differ in return types\n";
          return true;
        }
        vmt[i-2] = m;
        return true;
      }
    }

    void applicable_meths(std::list<Meth*>& l, Ident name, const std::list<Expr*> *args) const{
      std::vector<Meth*>::const_iterator m = vmt.begin();
      while(m != vmt.end()){
        if((*m)->ident() == name and (*m)->matches(args))
          l.push_back(*m);
        ++m;
      }
    }

    int meth_offset(const Meth *m) const{
      using namespace std;
      vector<Meth*>::const_iterator i = find_if(vmt.begin(),vmt.end(),
        bind1st(mem_fun(&Meth::sig_equals),m));
      if(i == vmt.end())
        return -1;
      else
        return i - vmt.begin() + 2; // 0 is supervmt, 1 is destructor, 2 is first meth
    }
  };

  extern unsigned int srcline;
  SymTabIt i = class_table().find(name);
  if(i == class_table().end()){
    _ *u = new _(name);
    class_table()[name] = VarInfo(u, srcline);
    return u;
  }else
    return class_table()[name].type;
}
