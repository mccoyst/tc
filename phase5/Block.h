#ifndef _BLOCK_DOT_H
#define _BLOCK_DOT_H

/*Steve McCoy
*/

#include "Stat.h"
#include <list>

/** Block of statements.
*/
class Block : public Stat{
  std::list<Stat*> *stats;
  friend class CInvoke;
  friend class Ctor;
public:
  /** { stats } */
  explicit Block(std::list<Stat*> *stats);
  ~Block();
  void dump() const;
  Node *analyze();
  void encode() const;
};

#endif
