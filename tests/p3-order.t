class A{
  B b;
}

int main(){
  A a;
  a = new A();
  a.b = new B();
  a.b.x = 42;
  return 42;
}

class B{
  int x;
}
