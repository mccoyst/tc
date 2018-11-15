class D extends B{ int n; }
class B extends Z{}
class Z extends Object{}

int main(){
  Object o;
  o = new D();
  B b;
  Z z;
  b = (B)z;
  b = (B)o;
  z = (Z)o;
  D d;
  d = (D)o;
  d.n = 42;
  return d.n;
}
