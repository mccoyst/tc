/*Steve McCoy
*/

#include <algorithm>
#include <functional>
#include <cassert>
#include <iostream>
#include "Block.h"
#include "utility.h"

Block::Block(std::list<Stat*> *s)
  : stats(s){
  assert(stats != 0);
}

Block::~Block(){
  std::transform(stats->begin(), stats->end(), stats->begin(), Delete<Stat>);
}

void Block::dump() const{
  std::cerr << "Block\n";
  std::for_each(stats->begin(), stats->end(), std::mem_fun(&Stat::dump));
  std::cerr << "End Block\n";
}

Node *Block::analyze(){
  for(std::list<Stat*>::iterator s = stats->begin(); s != stats->end(); ++s)
    try{
      *s = dynamic_cast<Stat*>((*s)->analyze());
    }catch(SyntaxError& e){
      // do nothing
    }
  return this;
}

void Block::encode() const{
  std::cout << "# Block\n";
  std::for_each(stats->begin(), stats->end(), std::mem_fun(&Stat::encode));
  std::cout << "# End Block\n";
}
