#ifndef _BREAK_DOT_H_
#define _BREAK_DOT_H_

/*Steve McCoy
*/

#include "Stat.h"

/** Leave while loop.
*/
class Break : public Stat{
  uint label;
public:
  Break();
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
