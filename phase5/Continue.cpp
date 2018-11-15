/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Continue.h"
#include "While.h"

Continue::Continue(){
}

void Continue::dump() const{
  std::cerr << " Continue\n";
}

Node *Continue::analyze(){
  if(While::inside()) label = While::label1();
  else error("continue is outside of a while loop", Severity::Error);
  return this;
}

void Continue::encode() const{
  std::cout << "# continue\n"
	  "  jmp    " << While::LblPrefix << label << '\n'
	  ;
}
