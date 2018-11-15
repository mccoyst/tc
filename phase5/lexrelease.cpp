/*Steve McCoy
* Debug versions of lexical analysis functions
*/

#include <iostream>
#include <boost/lexical_cast.hpp>
#include "tc.h"
#include "scanner.h"
#include "ast.h"
using namespace std;
#include "y.tab.h"

extern char *yytext;
extern unsigned int srcline;
extern int yylex();
extern void nonfatal(const char*);

int id_tok(){
	yylval.cstr = yytext;
	int val;
	return ((val = keyval(yytext)) == -1)
		? IDENTIFIER
		: val
		;
}

int int_tok(){
	try{
		yylval.n = boost::lexical_cast<unsigned int>(yytext);
		if(yylval.n > LargestInt)
			nonfatal("int literal value is too large");
		return INTEGER_LITERAL;
	}catch(const boost::bad_lexical_cast& blc){
		nonfatal("int literal value is too large");
		return INTEGER_LITERAL;
	}
}

int op1_tok(){
	return *yytext;
}

int op2_tok(){
	return EQ_OP;
}

int bad_tok(){
	return BAD;
}
