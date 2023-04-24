
#include <iostream>
#include <stdlib.h>

using namespace std;

// Can copy and move
class A {
public:
  A() { cout << "Create A\n"; }
  ~A() { cout << "Destroy A\n"; }
  A(const A&) { cout << "Copy A\n"; }
  A(A&&) { cout << "Move A\n"; }
};

A getA_unnamed()
{
  return A();
}

A getA_named(){ 
    A a; 
    return a;
}

A getA_duang()
{
  A a1;
  A a2;
  if (rand() > 42) {
    return a1;
  } else {
    return a2;
  }
}

A getA_duang2()
{
	A a1;
	A a2;
	return (rand() > 42 ? a1 : a2);
}



int main()
{
  auto a = getA_duang();
}

