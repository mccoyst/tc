class A{}
int main(){
  int x;
  A a;
  x = 12;
  a = x;
  x = a;
  x = a+x;
  x = a/x;
  a = a-a;
  a = a*a;
  x = !a;
  x = a < a;
}
