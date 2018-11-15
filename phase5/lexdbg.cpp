/*Steve McCoy
* Debug versions of lexical analysis functions
*/

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <boost/lexical_cast.hpp>
#include "tc.h"
#include "scanner.h"

extern char *yytext;
extern unsigned int srcline, lexerrs;
extern int yylex();
extern void yyerror(const char*);

bool dump_before, dump_after, optimize, warning;

extern int yylex(void), tokenpos;
extern unsigned int srcline;
extern std::string scanned_line;

void nonfatal(const char *s){
  ++lexerrs;
  std::cout.flush();
  std::replace(scanned_line.begin(), scanned_line.end(), '\t', ' ');
  std::cerr << "On line " << srcline << ": " << s << ":\n"
		<< scanned_line << '\n';
  std::cerr << std::setw(tokenpos+1) << "^\n";
}

int id_tok(){
	std::cout << srcline << ": "
		<< ((keyval(yytext) == -1) ? "Identifier" : "Keyword")
		<< " is '" << yytext << "'\n";
	return 1;
}

int int_tok(){
	unsigned int val;
	try{
		val = boost::lexical_cast<unsigned int>(yytext);
		std::cout << srcline << ": int literal is '"
			<< val << "'\n";
		if(val > LargestInt)
			nonfatal("int literal value is too large");
	}catch(const boost::bad_lexical_cast& blc){
		nonfatal("int literal value is too large");
	}
	return 1;
}

int op1_tok(){
	std::cout << srcline << ": '" << *yytext << "' is '" << *yytext << "'\n";
	return 1;
}

int op2_tok(){
	std::cout << srcline << ": '" << yytext << "' is '" << yytext << "'\n";
	return 1;
}

int bad_tok(){
	std::cout << srcline << ": Bad token is '" << yytext << "'\n";
	return 1;
}

int main(){
	while(yylex()) ;
	return 0;
}
