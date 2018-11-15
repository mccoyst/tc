class A{}
class B extends A{}
int main(){
  A a;
  B b;
  b = (B)a;
  if(b == null) return 42;
  else return -1;
}
