/*Steve McCoy
*/

#include "VDeclStat.h"
#include "SymTab.h"
#include "tc.h"
#include <algorithm>
#include <functional>
#include <cassert>
#include <iostream>
#include <sstream>

VDeclStat::VDeclStat(Type *t, std::list<VDecl*>* l)
	: type(t), decls(l){
	assert(type != 0 && decls != 0);
}

/** Calls error(), explaining redefinition of variable.
*/
void VDeclStat::redef_error(Ident name){
	std::ostringstream os;
	VarInfo vi = symbol_table()[name];
	os << "Redefinition of '" << name << "', originally declared as '"
		<< *vi.type << "' on line " << vi.decloc;
	error(os.str());
}

/** Prints type and each name to standard output.
*/
void VDeclStat::dump() const{
	std::cerr << " Declarations of type " << *type << " ( ";
	for_each(decls->begin(), decls->end(), std::mem_fun(&VDecl::dump));
	std::cerr << ")\n";
}

inline Type *merge_if_array(Type *t, const VDecl *vd){
  return (vd->dimensions() == 0) ? t : Type::array(t, vd->dimensions());
}

struct add_vdecl{
  VDeclStat *vds;
  add_vdecl(VDeclStat *v) : vds(v) {}
  
  void operator() (const VDecl *vd){
    VarInfo vi = VarInfo(merge_if_array(vds->type,vd), vd->line());
    SymEntry se = SymEntry(vd->name(), vi);
    std::pair<SymTabIt,bool> p = symbol_table().insert(se);
    if(!p.second) // not inserted
      vds->redef_error(vd->name());
  }
};

/** Attempt to add each name to the symbol table. Calls error() if a variable
* is re-declared.
*/
Node *VDeclStat::analyze(){
  std::for_each(decls->begin(), decls->end(), add_vdecl(this));
	if(dump_table) print_symbols();
	return this;
}

/** Encodes each declared variable.
*/
void VDeclStat::encode() const{
	std::for_each(decls->begin(), decls->end(), std::mem_fun(&VDecl::encode));
}
