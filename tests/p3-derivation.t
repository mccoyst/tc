class Base{
  int x;
  int y;
}

class Derived extends Base{
  int y;
  int z;
}

int main(){
  Base b;
  Derived d;
  b = new Base();
  d = new Derived();
  b.x = 9;
  b.y = 11;
  d.y = 5;
  d.z = 17;
  return b.x+b.y+d.y+d.z;
}
