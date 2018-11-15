class Node{
  Node next;
  int value;
}

int main(){
  Node n;
  n = new Node();
  n.value = 10;
  n.next = new Node();
  n.next.value = 12;
  n.next.next = new Node();
  n.next.next.value = 20;
  int total;
  while(!(n == null)){
    total = total + n.value;
    n = n.next;
  }
  return total;
}
