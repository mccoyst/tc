/*Steve McCoy
*/

#include <algorithm>
#include <cstring>
#include "tc.h"
using std::strcmp;

extern void yyparse();

bool dump_before, dump_after, dump_table, optimizing, warning;

void raise_if_flag(const char *arg){
	if(strcmp(arg, "-before") == 0)
		dump_before = true;
	else if(strcmp(arg, "-after") == 0)
		dump_after = true;
	else if(strcmp(arg, "-verbose") == 0)
		yydebug = 1;
	else if(strcmp(arg, "-table") == 0)
		dump_table = true;
	else if(strcmp(arg, "-optimize") == 0)
		optimizing = true;
	else if(strcmp(arg, "-warnings") == 0)
		warning = true;
  else if(strcmp(arg, "-classes") == 0)
    dump_classes = true;
}

int main(int argc, char *argv[]){
	std::for_each(argv, argv+argc, raise_if_flag);
	yyparse();
	return 0;
}
