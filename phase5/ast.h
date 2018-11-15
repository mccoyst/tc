#ifndef _AST_DOT_H_
#define _AST_DOT_H_

/** @file ast.h
* Defines all of the classes that can compose an abstract syntax tree.
* @author Steve McCoy
*/

// abstract classes

#include "Node.h"
#include "Stat.h"
#include "Expr.h"
#include "Unary.h"
#include "Binary.h"

// Concrete classes

#include "EmptyStat.h"
#include "ExprStat.h"
#include "Block.h"
#include "VDecl.h"
#include "VDeclStat.h"
#include "Var.h"
#include "Deref.h"
#include "Out.h"
#include "Ass.h"
#include "Equals.h"
#include "Less.h"
#include "Greater.h"
#include "Add.h"
#include "Sub.h"
#include "Mul.h"
#include "Div.h"
#include "Neg.h"
#include "Not.h"
#include "IntLit.h"
#include "NullLit.h"
#include "Control.h"
#include "IfElse.h"
#include "While.h"
#include "Return.h"
#include "Break.h"
#include "Continue.h"
#include "CBDecl.h"
#include "CDecl.h"
#include "CBDecl.h"
#include "FDecl.h"
#include "EmptyDecl.h"
#include "Del.h"
#include "Cast.h"
#include "New.h"
#include "FAccess.h"
#include "Dest.h"
#include "This.h"
#include "Super.h"
#include "Ctor.h"
#include "Param.h"
#include "Meth.h"
#include "MInvoke.h"
#include "CInvoke.h"
#include "NewArray.h"
#include "AAccess.h"
#include "CastA.h"

#endif
