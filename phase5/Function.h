#ifndef _METHOD_DOT_H
#define _METHOD_DOT_H

#include "Ident.h"
#include <list>
#include "Expr.h"
#include "Param.h"
#include "Block.h"

class Function{
protected:
  static const Function *cur;
  Type *ret;
  Ident name;
  std::list<Param*> *params;
  Block *body;
  const Type *enclosing;
public:
  Function(Type *ret, Ident name, std::list<Param*> *params, Block *body);
  virtual ~Function(){}
  const Type *rtype() const;
  Ident ident() const;
  const Type *param_type(Ident) const;
  int nparams() const;
  int param_pos(Ident) const;
  std::string munge() const;
  bool matches(const std::list<Expr*>*) const;
  bool sig_equals(const Function*) const;
  bool more_specific(const Function*) const;
  void dump(bool everything) const;
  void analyze_params();
  void analyze_body();
  void encode_body() const;
  bool duplicated_param_name(Ident&) const;

  static const Function *current();
  static void current(const Function*);

  class Make_current{
    const Function *old;
  public:
    Make_current(const Function *f);
    ~Make_current();
  };
};

inline const Function *Function::current(){
  return cur;
}
inline void Function::current(const Function *m){
  cur = m;
}

inline Function::Make_current::Make_current(const Function *f)
  : old(Function::current()){
  Function::current(f);
}
inline Function::Make_current::~Make_current(){
  Function::current(old);
}

#endif
