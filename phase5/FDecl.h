#ifndef _FDECL_DOT_H_
#define _FDECL_DOT_H_

/*Steve McCoy
*/

#include <list>
#include "CBDecl.h"
#include "Type.h"
#include "VDecl.h"

/** Class field(s) declaration.
*/
class FDecl : public CBDecl{
  friend class CDecl;
  Type *type;
  std::list<VDecl*> *decls;
public:
  FDecl(Type *type, std::list<VDecl*> *decls);
  void dump() const;
  Node *analyze();
  void encode() const;
  void add_to(Type *t);
  bool complete() const;
  std::string str() const;
};

#endif
