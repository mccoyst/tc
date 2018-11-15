#ifndef _VDECLSTAT_DOT_H_
#define _VDECLSTAT_DOT_H_

#include "Stat.h"
#include "Type.h"
#include "VDecl.h"
#include <list>

/** Main variable declaration statement. Declares a list of variables of a type.
*/
class VDeclStat : public Stat{
  Type *type;
  std::list<VDecl*>* decls;
  friend class add_vdecl;
public:
  /** type decls; */
  VDeclStat(Type *type, std::list<VDecl*>* decls);
  void redef_error(Ident name); // seems like this should be a member of VDecl
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
