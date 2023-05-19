#include <iostream>

using namespace std;

void (* functions[20])();


void func0(){
	cout << "function 0"<<endl;
	return;
}
void func1(){
	cout << "function 1"<<endl;

	return;
}
void func2(){
	cout << "function 2"<<endl;
	return;
}

int main()
{

	functions[0x0] = func0;
	functions[0x1] = func1;
	functions[0x2] = func2;
	functions[0x1]();
}