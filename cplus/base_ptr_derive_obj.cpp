#include <iostream>
class A
{
public:
	virtual void f() { std::cout << "base function" << endl; }
};

class B : public A
{
public:
	virtual void f() { std::cout << "derived function" << endl; }
};

int main(int argc, char *argv[])
{
	A* a = new B();
	(*a).f();
	return 0;
}