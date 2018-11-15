#ifndef _SYMTAB_HEADER_FILE_
#define _SYMTAB_HEADER_FILE_

/** @file SymTab.h
* Declarations required for accessing the symbol tables.
* @author Steve McCoy
*/

#include <map>
#include "Ident.h"

/** Variable info. Container for the variable's type and declaration location
*/
struct VarInfo{
	Type *type;		/**< variable's type */
	unsigned int decloc;	/**< variable's declaration location */
	VarInfo() : type(0), decloc(0) {}
	VarInfo(Type *t, unsigned int d) : type(t), decloc(d) {}
};

/** The symbol table's type */
typedef std::map<Ident, VarInfo> SymTab;
/** The type of a symbol table entry */
typedef SymTab::value_type SymEntry;
/** The type of an iterator over the symbol table */
typedef SymTab::iterator SymTabIt;

/** Function to obtain the symbol table */
extern SymTab& symbol_table();
/** Function to obtain the class table */
extern SymTab& class_table();
/** Function to print all of the symbols to standard error */
extern void print_symbols();
/** Function to print all of the classes to standard error */
extern void print_classes();

#endif

