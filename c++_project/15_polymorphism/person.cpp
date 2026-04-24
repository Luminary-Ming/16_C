#include "person.h"
#include <iostream>

using namespace std;  // 打开命名空间

// 使用初始化列表的方式初始化成员变量
Person::Person(const char *name, int age) : m_name(name), m_age(age) 
{
	cout << "Person  类的构造函数被调用了 !" << endl;
}

// 打印信息
void Person::show() const
{
	cout << "姓名: " << m_name << " 年龄: " << m_age << endl;
}

// 析构函数
Person::~Person()
{
	cout << "Person  类的析构函数被调用了 !" << endl;
}