/*Steve McCoy
*/

#include <cassert>
#include <iostream>
#include "Break.h"
#include "While.h"

Break::Break(){
}

void Break::dump() const{
  std::cerr << " Break\n";
}

Node *Break::analyze(){
  if(While::inside()) label = While::label2();
  else error("break is outside of a while loop", Severity::Error);
  return this;
}

void Break::encode() const{
  std::cout << "# break\n"
    "  jmp    " << While::LblPrefix << label << '\n'
    ;
}
