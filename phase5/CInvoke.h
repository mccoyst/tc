#ifndef _CINVOKE_DOT_H_
#define _CINVOKE_DOT_H_

/*Steve McCoy
*/

#include "Stat.h"
#include "Expr.h"
#include "Ctor.h"
#include <list>

class CInvoke : public Stat{
  friend class Ctor;
public:
  enum Invoker { This, Super };
  CInvoke(Invoker i, std::list<Expr*> *args);
  void dump() const;
  Node *analyze();
  void encode() const;
  static Ctor *most_specific(std::list<Ctor*>& ctors);
  bool calls(const Ctor*) const;
private:
  Invoker invoker;
  std::list<Expr*> *args;
  Ctor *ctor;
};

#endif
