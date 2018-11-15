class A{}
int main(){
  A a,b;
  a = b = new A();
  if(a == b) out 42;
  else out -1;
  b = new A();
  if(a == b) out -1;
  else out 42;
}
