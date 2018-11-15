#ifndef _CDECL_DOT_H_
#define _CDECL_DOT_H_

/*Steve McCoy
*/

#include <list>
#include "Node.h"
#include "CBDecl.h"
#include "Ident.h"
#include "Type.h"

class FDecl;

/** Class Declaration.
*/
class CDecl : public Node{
  Ident name;
  Type *type, *super;
  std::list<CBDecl*> *body;
  bool good;
  void add_members();
public:
  CDecl(Ident name, Type *super, std::list<CBDecl*> *body);
  void dump() const;
  Node *analyze();
  void encode() const;
  void update();
};

#endif
