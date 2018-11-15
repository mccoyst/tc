#ifndef _VAR_DOT_H_
#define _VAR_DOT_H_

/*Steve McCoy
*/

#include "Ident.h"
#include "Expr.h"

/** Variable.
*/
class Var : public Expr{
    Ident id;
public:
    /** variable named name */
    explicit Var(Ident name);
    /** Reports on a variable that's been used, but hasn't been declared. */
    void undef_error(Ident name);
    /** The name of the variable. */
    Ident name() const;
    /** Hack for cast expression */
    void type(const Type *t);
    void dump() const;
    Node *analyze();
    void encode() const;
};

#endif
