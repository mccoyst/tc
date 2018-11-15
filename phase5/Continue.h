#ifndef _CONTINUE_DOT_H_
#define _CONTINUE_DOT_H_

/*Steve McCoy
*/

#include "Stat.h"

/** Skip rest of while loop's block.
*/
class Continue : public Stat{
  uint label;
public:
  Continue();
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
