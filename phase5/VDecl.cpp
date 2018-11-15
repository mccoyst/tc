/*Steve McCoy
*/

#include <iostream>
#include "VDecl.h"

/** Prints the identifer associated with the variable declaration to cerr.
*/
void VDecl::dump() const{
	std::cerr << name();
	for(uint i = 0; i < dimensions(); ++i)
	  std::cerr << "[]";
  std::cerr << ' ';
}

/** Nothing to analyze; simply returns \a this.
*/
Node *VDecl::analyze(){
	return this;
}

void VDecl::encode() const{
  std::cout << "# Declaration of '" << id << "'\n"
	  "\t.data\n"
	  "\t.comm " << id << ", 4\n"
	  "\t.text\n"
	  ;
}
