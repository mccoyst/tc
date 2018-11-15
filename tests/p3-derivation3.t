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
  b = new Derived();
  b.x = 9;
  out b.x;
  b.y = 11;
  out b.y;
  Derived d;
  d = (Derived)b;
  d.y = 20;
  out d.y;
  d.z = 2;
  out d.z;
  out b.x + b.y;
  out d.y + d.z;
  return b.x+b.y+d.y+d.z;
}
