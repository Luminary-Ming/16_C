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

// A 类, 以 public 的方式虚继承 Base 类
// 现在 A 类中可以使用两个成员变量, 一个是 m_a, 一个是 m_base
class A : virtual public Base
{
protected:
	int m_a;

public:
	// 构造函数 (初始化列表方式初始化成员变量)
	A(int a, int base) : m_a(a), Base(base) {};
};

// B 类, 以 public 的方式虚继承 Base 类
// 现在 B 类中可以使用两个成员变量, 一个是 m_a, 一个是 m_base
class B : virtual public Base
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
	// C++ 规定: 虚基类的构造函数必须由最终创建对象的那个类 (即最派生类) 来直接调用, 而不是由中间层类 (A、B) 去调用
	// C 是最派生类 (继承树最底端的类), 必须由它调用 Base 的构造函数, 而 A 和 B 对 Base 的构造函数调用会被忽略
	// 所以 C 的构造函数可以显式写出 Base(base), 这是合法的
	// 最终 C 对象中只有一个 m_base 成员, 它的值由 C 传入的 base 决定
	// A 和 B 的构造函数虽然也写了 Base(base+5) 和 Base(base-5), 但这些调用在构造 C 的对象时被忽略, 只有 C 调用的 Base(base) 生效
	// 所以 A::m_base 和 B::m_base 访问的是同一块内存地址, 值也相同 (由 C 设置的 base)
	C(int a, int b, int c, int base) : A(a, base + 5), B(b, base - 5), m_c(c), Base(base) {};
	void show() const  // 打印成员信息
	{
		cout << m_a << ", " << m_b << ", " << m_c << ", " << endl;

		cout << "A::m_base " << A::m_base << endl;
		cout << "B::m_base " << B::m_base << endl;

		cout << "(void *)&(A::m_base) " << (void *)&(A::m_base) << endl;
		cout << "(void *)&(B::m_base) " << (void *)&(B::m_base) << endl;
	}
};
/*
	总结

	继承方式            C 能否直接调用 Base 构造函数   原因
	普通多继承 (非虚)   不能                           Base 不是 C 的直接基类; C 中有两份 Base, 只能由 A 和 B 分别构造
	虚继承 (virtual)    可以                           虚基类只有一份, 且必须由最派生类 C 直接负责初始化
*/
/*
	22, 33, 44,
	A::m_base 55
	B::m_base 55
	(void *)&(A::m_base) 0x7ffce3d5c6c0
	(void *)&(B::m_base) 0x7ffce3d5c6c0
*/


/*
内存布局示意 :

在 A 和 B 虚继承中:
	vbptr: 虚基类表指针, 指向这个共享的 Base 子对象;
	A::m_base 和 B::m_base 实际上是同一个Base子对象, 通过 vbptr 间接访问;
	所以取地址 &A::m_base 就是取共享 Base 中的 m_base 地址, 所以相同;

	C 对象内存布局：
	┌─────────────────┐
	│ A::vbptr        │  (8 字节)
	├─────────────────┤
	│ A::m_a          │  (4 字节)
	├─────────────────┤  
	│ B::vbptr        │  (8 字节)
	├─────────────────┤
	│ B::m_b          │  (4 字节)
	├─────────────────┤
	│ C::m_c          │  (4 字节)
	├─────────────────┤  ← 0x7ffcdeb50ae8
	│ Base::m_base    │  (4 字节)
	└─────────────────┘


总结
	继承类型	Base 子对象数量	    m_base 地址情况
	非虚继承	2 份（各自独立）	A::m_base 和 B::m_base 不同址
	虚继承	    1 份（共享）	    A::m_base 和 B::m_base 同址

*/


#endif