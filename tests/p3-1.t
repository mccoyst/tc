class A {}
class B extends Object {}
class C{
  A a;
  B b;
}
class D extends B { int x; C c; }
class E extends D { int y; }
class F extends E { int x; D c; }

int main(){
  A a;
  B b, db, eb;
  C c;
  D d, fd;
  E e;
  F f;
  a = new A();
  b = new B();

  c = new C();
  c.a = new A();
  c.b = new B();

  d = new D();
  d.x = 12;

  eb = e = new E();
  e.y = 4;
  e.x = 6;

  fd = f = new F();
  f.x = 3;
  f.y = 10;
  f.c = new D();
  fd.x = 8;
  fd.c = new C();
  ((D)f).x = 7;
  return d.x+e.y+e.x+f.x+f.y+fd.x;
}
