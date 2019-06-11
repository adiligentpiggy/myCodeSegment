#include <iostream>

using namespace std;

class A {
	int a;
};

int main(void) {
    A a[2], b, c;
	a[0] = b;
	a[1] = c;
	
	cout << &a[0] << endl;
	cout << a << endl;
    
    return 0;
}
