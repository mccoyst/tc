#ifndef _CBDECL_DOT_H_
#define _CBDECL_DOT_H_

/*Steve McCoy
*/

#include "Node.h"
#include "Type.h"

/** Class member, constructor, or destructor declaration.
*/
class CBDecl : public Node{
  friend class CDecl;
public:
  /** Add self as member of type t */
  virtual void add_to(Type *t) = 0;
};

#endif
