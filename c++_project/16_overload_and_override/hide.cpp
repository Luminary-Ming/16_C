#include <iostream>

using namespace std;

/*
	遮蔽（Hide）

	存在与继承关系中
	函数名相同，参数列表不关心，函数实现不同，返回值不关心
	基类函数没有用 virtual 关键字修饰
*/

// 基类
class Base
{
public:
	void test_1(int a = 2233)  // 没有使用 virtual 关键字修饰，将来会发生遮蔽
	{
		cout << "Base::test_1() Is Called!" << endl;
		cout << "Base::test_1().a = " << a << endl;
	}

	virtual void test_2(int a = 2233)  // 有 virtual 关键字修饰，但要保证派生类中的参数列表也相同，否则也会发生遮蔽
	{
		cout << "Base::test_2() Is Called!" << endl;
		cout << "Base::test_2().a = " << a << endl;
	}

	void test_3()  // 没有使用 virtual 关键字修饰，将来会发生遮蔽
	{
		cout << "Base::test_3() Is Called!" << endl;
	}

};

// 派生类
class Derived : public Base
{
public:
	void test_1()  // 1.函数名相同，但是参数列表不同，并且基类中没有用 virtual 关键字修饰
	{
		cout << "Derived::test_1() Is Called!" << endl;

		// 会造成无限递归，段错误（核心已转储）
		//test_1();

		// 因为因为派生类和基类的参数列表不同，并且基类中的 test_1 方法没有用 virtual 关键字修饰，发生遮蔽
		// 所以如果派生类想调用基类中的 test_1，那就需要加指定
		Base::test_1();
	}

	void test_2()  // 2.函数名相同，基类中也有用 virtual 关键字修饰，但是参数列表不同
	{
		cout << "Derived::test_2() Is Called!" << endl;

		// 会造成无限递归，段错误（核心已转储）
		//test_2();

		// 因为派生类和基类的参数列表不同，所以不是重写，而是遮蔽
		// 所以如果派生类想调用基类中的 test_2，那就需要加指定
		Base::test_2();
	}

	void test_3()  // 3.函数名相同，参数列表相同，但是基类中没有用 virtual 关键字修饰
	{
		cout << "Derived::test_3() Is Called!" << endl;

		// 会造成无限递归，段错误（核心已转储）
		//test_3();

		// 因为基类中的 test_3 方法没有用 virtual 关键字修饰，发生遮蔽
		// 所以如果派生类想调用基类中的 test_3，那就需要加指定
		Base::test_3();
	}
};

int main(void)
{
	Base base;

	Derived derived;

	derived.test_1();
	derived.test_2();
	derived.test_3();

	return 0;
}
/*
	Derived::test_1() Is Called!
	Base::test_1() Is Called!
	Base::test_1().a = 2233

	Derived::test_2() Is Called!
	Base::test_2() Is Called!
	Base::test_2().a = 2233

	Derived::test_3() Is Called!
	Base::test_3() Is Called!
*/