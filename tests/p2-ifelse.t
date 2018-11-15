int main(){
	int a, b;
	a = b = 42;
	if(!(a == b)) out -42;
	else out a;
	a = a -1;
	if(1 > 0) if(a < b) out b; else out a;
	else out -42;
}
