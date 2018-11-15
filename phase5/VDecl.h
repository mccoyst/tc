#ifndef _VDECL_DOT_H_
#define _VDECL_DOT_H_

/*Steve McCoy
*/

#include "Node.h"
#include "Ident.h"

/** Variable declaration.
*/
class VDecl : public Node{
  Ident id;
  uint dims;
public:
  /** declaration of variable named name with d > 0 for additional dimensions */
  explicit VDecl(Ident name, uint d) : id(name), dims(d) {}
  /** variable's name */
  Ident name() const{ return id; }
  /** number of additional dimensions */
  uint dimensions() const{ return dims; }
  /** increments the additional dimensions */
  void add_dimension(){ ++dims; }
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
