#ifndef _CONTROL_DOT_H_
#define _CONTROL_DOT_H_

/*Steve McCoy
*/

#include "Expr.h"
#include "Stat.h"

/** Abstract control statement, controlled by an expression.
*/
class Control : public Stat{
protected:
  /** The control's controlling expression. */
  Expr *cond;
public:
  /** Control with cond */
  explicit Control(Expr *cond);
  /** Calls error(), explaining that cond isn't of the type expected. */
  void mismatch_error(const std::string& name, const Type *expected);
  /** Returns true if cond is a compile-time constant */
  bool is_constexpr() const;
  Node *analyze();
};

#endif
