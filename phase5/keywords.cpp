/*Steve McCoy
*/

#include <algorithm>
#include <cstring>
#include <tr1/array>
	// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2003/n1479.html
#include "tc.h"
using namespace std;
#include "ast.h"
#include "y.tab.h"

namespace{
	struct KeyTok{
		char *key;
		int tok;
	};
	bool operator==(const KeyTok& kt, const char *key){
		return strcmp(kt.key, key) == 0;
	}
	typedef std::tr1::array<KeyTok, 16> KTArray;
	const KTArray keywords = {{
		{"class", CLASS},
		{"delete", DELETE},
		{"else", ELSE},
		{"extends", EXTENDS},
		{"if", IF},
		{"int", INT},
		{"main", MAIN},
		{"new", NEW},
		{"out", OUT},
		{"return", RETURN},
		{"super", SUPER},
		{"this", THIS},
		{"while", WHILE},
		{"break", BREAK},
		{"continue", CONTINUE},
    {"null",NULL_LITERAL},
	}};
}

int keyval(const char *s){
	KTArray::const_iterator kt = std::find(keywords.begin(), keywords.end(), s);
	return (kt == keywords.end()) ? -1 : kt->tok;
}
