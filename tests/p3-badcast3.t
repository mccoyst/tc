class A {}
class B extends A {}
class C extends B {}
int main(){
  A a;
  B b;
  C c;
  a = new B();
  b = (B) a;
  c = (C) b;
}
