#include <iostream>
#include "student.h"

int Student::sum = 0;  // 静态成员变量一定要有初始化

// 类成员函数的实现 (类名::函数名)
void Student::show(void)
{
	std::cout << m_name << "年龄: " << m_age << std::endl;
}

// 类的成员函数可以访问 private 修饰的成员变量
void Student::setName(const char *name)
{
	m_name = name;
}

// 类的成员函数可以访问 private 修饰的成员变量
void Student::setAge(int age)
{
	sum++;
	m_age = age;
}

int Student::getSum()
{
	// return this->sum 报红, 静态成员函数没有 this 指针
	// 静态成员函数只能访问静态成员变量, 不能访问其他成员变量

	return sum;
}

int Student::getAge() const
{
	return m_age;
}

const char *Student::getName() const
{
	return m_name;
}