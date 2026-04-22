#include <iostream>

using namespace std;  // 打开命名空间

// 函数声明
// 注意: 默认参数只能在声明或定义中出现一次，通常放在声明中
int add(int a, int b);             // 无默认参数
int sub(int a, int b = 7);         // b 有默认值 7, a 没有 (合法, 从右向左连续)
int mul(int a = 13, int b = 7);    // a b 都有默认值 (合法)
int divi(int a = 13, int b = 7);   // a b 都有默认值 (合法)
//int test(int a = 13, int b);     // 不合法, 默认参数必须从右向左连续提供, 并且不能跳过中间参数; b 没有默认值, 而 a 有, 编译错误


// 函数实现
int add(int a = 13, int b = 7)  // 有默认参数 (函数声明不指定, 函数实现中指定)
{
	return a + b;
}

int sub(int a, int b)  // 没有默认参数 (已在声明中指定 b=7)
{
	return a - b;
}

int mul(int a, int b)  // 没有默认参数 (已在声明中指定)
{
	return a * b;
}

int divi(int a, int b)  // 没有默认参数 (已在声明中指定)
{
	return a / b;
}

int main(void)
{
	int a = 0, b = 0;

	cout << "输入 a b 的值" << endl;

	cin >> a >> b;

	// 使用实参调用, 默认参数不会被使用
	cout << "add = " << add(a, b) << endl;
	cout << "sub = " << sub(a, b) << endl;
	cout << "mul = " << mul(a, b) << endl;
	cout << "divi = " << divi(a, b) << endl;

	cout << "----------------" << endl;

	// 不提供实参, 使用默认参数 (前提是函数声明或实现中有默认参数)
	cout << "add = " << add() << endl;    // 默认参数 (13, 7)
	cout << "sub = " << sub(a) << endl;   // b 使用默认值 7
	cout << "mul = " << mul() << endl;    // 默认参数 (13, 7)
	cout << "divi = " << divi() << endl;  // 默认参数 (13, 7)

	return 0;
}
/*
	输入 a b 的值
	10 5
	add = 15
	sub = 5
	mul = 50
	divi = 2
	----------------
	add = 20
	sub = 3
	mul = 91
	divi = 1
*/