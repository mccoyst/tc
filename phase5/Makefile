# Steve McCoy

CXX = g++
CPPFLAGS = 
CXXFLAGS = -std=c++98 -Wall
LDFLAGS = 
LEX = flex
LFLAGS = 
YACC = bison
YFLAGS = -yd

classes = Add.o Ass.o Binary.o Block.o Deref.o Div.o EmptyStat.o Equals.o\
	Expr.o ExprStat.o Greater.o IntLit.o Less.o Mul.o Neg.o Node.o Not.o\
	Out.o Sub.o SymTab.o Type.o Unary.o Var.o VDecl.o VDeclStat.o Control.o\
	IfElse.o While.o Return.o Break.o Continue.o Ident.o EmptyDecl.o\
	Del.o New.o FAccess.o NullLit.o Cast.o FDecl.o CDecl.o Dest.o\
	This.o Super.o Ctor.o Param.o Meth.o MInvoke.o CInvoke.o Function.o\
	TypeInt.o TypeUnknown.o TypeNull.o TypeObject.o TypeUser.cpp TypeArray.o\
	NewArray.o AAccess.o CastA.o

tc: $(classes) tc.o lexrelease.o keywords.o lex.yy.c y.tab.c ast.h
	$(CXX) $(LDFLAGS) $^ -o $@

include dependencies.txt

lexdbg: lexdbg.o keywords.o lex.yy.c
	$(CXX) $(LDFLAGS) $^ -o $@

lex.yy.c: scanner.l scanner.h y.tab.h tc.h
	$(LEX) $(LFLAGS) scanner.l

y.tab.c y.tab.h: grammar.y ast.h Type.h 
	$(YACC) $(YFLAGS) grammar.y

lexdbg.o: scanner.h lexdbg.cpp y.tab.h

lexrelease.o: scanner.h lexrelease.cpp y.tab.h

keywords.o: tc.h keywords.cpp

.PHONY: clean tidy dependencies
clean: tidy
	rm -f lexdbg tc
tidy:
	rm -f *.o lex.yy.c y.tab.c y.tab.h grammar.tab.c
dependencies:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM *.cpp > dependencies.txt

