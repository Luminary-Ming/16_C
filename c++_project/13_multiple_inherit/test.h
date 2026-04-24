#ifndef TEST_H
#define TEST_H

#include <iostream>
using namespace std;

// Base 类
class Base
{
protected:
	int m_base;

public:
	// 构造函数 (初始化列表方式初始化成员变量)
	Base(int base) : m_base(base) {};
};

// A 类, 以 public 的方式继承 Base 类
// 现在 A 类中可以使用两个成员变量, 一个是 m_a, 一个是 m_base
class A : public Base
{
protected:
	int m_a;

public:
	// 构造函数 (初始化列表方式初始化成员变量)
	A(int a, int base) : m_a(a), Base(base) {};
};

// B 类, 以 public 的方式继承 Base 类
// 现在 B 类中可以使用两个成员变量, 一个是 m_a, 一个是 m_base
class B : public Base
{
protected:
	int m_b;

public:
	// 构造函数 (初始化列表方式初始化成员变量)
	B(int b, int base) : m_b(b), Base(base) {};
};


// C 类
// 以 public 的方式继承 A 类和 B 类
// A 和 B 都继承了 Base, 而 C 又同时继承了 A 和 B (多继承中的 "菱形继承")
class C : public A, public B
{
private:
	int m_c;

public:
	// 构造函数 (初始化列表方式初始化成员变量)
	// 注意: Base 类不是 C 类的基类, 不能调用 Base 类的构造函数 (虚继承除外)
	C(int a, int b, int c, int base) : A(a, base + 5), B(b, base - 5), m_c(c) {};
	void show() const  // 打印成员信息
	{
		cout << m_a << ", " << m_b << ", " << m_c << ", " << endl;

		// 因为 A 类和 B 类都继承了 Base 类中的 m_base 成员变量
		// 所以在使用的时候要明确指定是 A 类中的 m_base 还是 B 类中的 m_base
		cout << "A::m_base " << A::m_base << endl;
		cout << "B::m_base " << B::m_base << endl;

		cout << "(void *)&(A::m_base) " << (void *)&(A::m_base) << endl;
		cout << "(void *)&(B::m_base) " << (void *)&(B::m_base) << endl;
	}
};
// 没有使用 virtual 继承, Base 类在 C 的对象中存在两份独立的子对象: 一份来自 A 路径, 另一份来自 B 路径
// 因此 C 的对象中会有两个完全独立的 m_base 成员变量 (分别属于 A::m_base 和 B::m_base), 互不影响
/*
	22, 33, 44,
	A::m_base 60
	B::m_base 50
	(void *)&(A::m_base) 0x7ffcdeb50ae0
	(void *)&(B::m_base) 0x7ffcdeb50ae8
*/


/*

地址分别为 0x7ffcdeb50ae0 和 0x7ffcdeb50ae8, 相差 8 字节
内存布局示意 :

	C 对象内存布局：
	┌─────────────────┐  ← 0x7ffcdeb50ae0
	│ A::m_base       │  (4 字节)
	├─────────────────┤
	│ A::m_a          │  (4 字节)
	├─────────────────┤  ← 0x7ffcdeb50ae8
	│ B::m_base       │  (4 字节)
	├─────────────────┤
	│ B::m_b          │  (4 字节)
	├─────────────────┤
	│ C::m_c          │  (4 字节)
	└─────────────────┘

*/


#endif