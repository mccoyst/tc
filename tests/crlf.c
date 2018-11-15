#include <stdio.h>
int main(void){
	FILE *f = fopen("crlf.t", "w");
	fprintf(f, "int main(){\n"
		"	int abc; // lf\n"
		"	int def; // cr\r"
		"	int ghi; // crlf\r\n"
		"	int jkl;\n"
		"}");
}
