#include <iostream>

using namespace std;

/*
	重写 (Override)

	存在与继承关系中
	函数名相同，参数列表相同，函数实现不同，返回值兼容
	基类函数用 virtual 关键字修饰
*/

// 基类
class Base
{
public:
	// 没有使用 virtual 修饰，如果基类中的同名函数，会发生遮蔽
	//void show(int a, double b);
	virtual void show(int a, double b)
	{
		cout << "Base::show() Is Called!" << endl;
		cout << "a = " << a << "b = " << b << endl;
	}
};

// 派生类
class Derived : public Base
{
public:
	void show(int a, double b)  // 派生类重写
	{
		cout << "Derived::show() Is Called!" << endl;
		cout << "a = " << a << "b = " << b << endl;
	}
};

int main(void)
{
	Base base;

	Derived derived;

	Base *p = &derived;  // 多态

	p->show(22, 22.22);

	p = &base;

	p->show(33, 33.33);

	return 0;
}
/*
	Derived::show() Is Called!
	a = 22b = 22.22
	Base::show() Is Called!
	a = 33b = 33.33
*/