class D extends B{
  int x;
}
class A{
  int x;
}

int main(){
  D d;
  A a;
  B b;
  a = b = d = new D();
  d.x = 1;
  b.x = 2;
  a.x = 3;
  out d.x;
  out b.x;
  out a.x;
}
class B extends A{
  int x;
}
