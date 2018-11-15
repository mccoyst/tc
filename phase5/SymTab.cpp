/*Steve McCoy
*/

#include <algorithm>
#include <iostream>
#include "Type.h"
#include "SymTab.h"


SymTab& symbol_table(){
	static SymTab _;
	return _;
}

SymTab& class_table(){
	static SymTab _;
	static bool once = false;
	if(!once){
	  _[Ident("Object")] = VarInfo(Type::object(),0);
	  once = true;
  }
	return _;
}

inline void print_symbol(SymEntry k){
	std::cerr << '(' << k.first << ','
		<< *k.second.type << ",@" << k.second.decloc << ")\n";
}

inline void print_class(SymEntry k){
  k.second.type->dump();
}

inline void print_table(const SymTab& st){
	std::for_each(st.begin(), st.end(), print_symbol);
}

void print_symbols(){
	std::cerr << "\n--Symbol Table--\n";
	print_table(symbol_table());
	std::cerr << '\n';
}

void print_classes(){
	std::cerr << "\n--Class Table--\n";
	std::for_each(class_table().begin(), class_table().end(), print_class);
	std::cerr << '\n';
}
