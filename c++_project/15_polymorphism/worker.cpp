#include "worker.h"

using namespace std;

// 构造函数以初始化列表的方式初始化成员变量
Worker::Worker(const char *name, int age, float salary) : Person(name, age), m_salary(salary)  // 构造函数
{}

// 打印信息 (必须对基类中的同名函数 show 进行重写)
// 方法的重写:
// 1.存在于继承关系中
// 2.基类函数是虚函数 (virtual 关键字修饰)
// 3.基类和派生类的函数名相同, 参数列表相同, 返回值相同, 但函数实现不同
//
// 关键说明:
// final 关键字 (C++11 起): 禁止派生类进一步重写该虚函数: virtual void func() final;
// 访问权限可以不同: 派生类可以将 public 基类虚函数重写为 private (虽不推荐，但语法允许)。多态调用时仍会执行派生类版本, 访问权限由静态类型决定。
// 若基类函数非虚, 派生类定义同名函数会遮蔽基类版本, 而不是重写。此时通过基类指针调用同名函数的仍是基类函数 (没有多态了)
// 构造函数、静态成员函数、友元函数不能被重写 (前两者不参与多态, 友元函数不是成员函数)
void Worker::show() const
{
	cout << "工人姓名: " << m_name << ", 年龄: " << m_age << ", 薪水: " << m_salary << endl;
}