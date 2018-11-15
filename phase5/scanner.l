%{
/*Steve McCoy
* Scanner for the T programming language
*/

#include <string>

#include "scanner.h"

unsigned int srcline = 1, lexerrs = 0;
%}

%option unput

lineterm	((\r|\n)|\r\n)
whitespace	([ \t\f])
comment		("//"[^\r\n]*)
letter		([A-Za-z_])
digit		([0-9])
identifier	({letter}({letter}|{digit})*)
int_lit		({digit}+)
separator	([\[\](){};,.~])
operator1	([-=+>!/<*])
operator2	(==)

%%

{lineterm}    { ++srcline; }
{whitespace}  { /* Ignore */ }
{comment}     { /* Ignore */ }
{identifier}  { return id_tok(); }
{int_lit}     { return int_tok(); }
{separator} |
{operator1}   { return op1_tok(); }
{operator2}   { return op2_tok(); }
.             { return bad_tok(); }

%%

extern "C"{
	int yywrap() { return 1; }
	void yyinit() { unput('\n'); }
}

