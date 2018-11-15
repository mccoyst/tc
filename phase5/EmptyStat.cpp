/*Steve McCoy
*/

#include <iostream>
#include "EmptyStat.h"

Stat *empty_stat(){
  static class _ : public Stat{
  public:
    void dump() const{std::cerr << " Empty statement\n"; }
    Node *analyze(){ return this; }
    void encode() const{ std::cout << "# Empty statement\n"; }
  } es;
  return &es;
}
