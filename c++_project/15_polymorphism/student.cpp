#include "student.h"
#include <iostream>

using namespace std;  // 打开命名空间

// 派生类并不继承基类的构造函数, 所以派生类需要调用基类的构造函数, 从而初始化继承过来的属性
Student::Student(const char *name, int age, float score) : Person(name, age), m_score(score)
{
	cout << "Student 类的构造函数被调用了 !" << endl;
}

// 打印信息
void Student::print() const
{
	Person::show();  // 因为 show() 在 Person 类中是以 public 修饰的, 所以 Student 类可以继承, 并且直接调用
	// 这里如果不指定 "Person::show()" 调用, 直接 "show()" 调用的话, 调用的是自己类中的 show() 方法

	cout << "成绩是: " << m_score << endl;
}


// 打印信息 (基类和派生类函数重名情况)
void Student::show() const
{
	Person::show();  // 当派生类的函数名和基类的函数名相同时, 调用基类的函数需要用 "基类::函数名" 调用
	// 这里如果不指定 "Person::show()" 调用, 直接 "show()" 调用的话, 调用的是自己类中的 show() 方法

	cout << "成绩是: " << m_score << endl;
}

// 析构函数
Student::~Student()
{
	cout << "Student 类的析构函数被调用了 !" << endl;
}