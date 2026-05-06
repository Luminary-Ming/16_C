#include <iostream>

using namespace std;

/*
	重载 (Overload)

	在同一个类中, 函数名相同, 参数列表不同, 与函数返回值无关

	参数列表不同:
		参数的个数不同
		参数的类型不同
		参数的顺序不同
*/

class Overload
{
public:
	void test()
	{
		cout << "[1]test() Is Called !" << endl;
	}
	void test(int a)
	{
		cout << "[2]test() Is Called !" << endl;
		cout << "a = " << a << endl;
	}
	void test(int a, double b)
	{
		cout << "[3]test() Is Called !" << endl;
		cout << "a = " << a << " b = " << b << endl;
	}

	void test(double b, int a)
	{
		cout << "[4]test() Is Called !" << endl;
		cout << "a = " << a << " b = " << b << endl;
	}
};

int main(void)
{
	Overload o;
	o.test();
	o.test(22);
	o.test(22, 33.33);
	o.test(33.33, 22);

	return 0;
}

/*
	[1]test() Is Called !
	[2]test() Is Called !
	a = 22
	[3]test() Is Called !
	a = 22 b = 33.33
	[4]test() Is Called !
	a = 22 b = 33.33
*/