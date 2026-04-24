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

// 友元函数的实现 (友元函数不是类的成员函数, 只是需要声明在类内部, 本质还是普通函数)
// 函数实现不需要 "Student::printObj" 这种写法, 因为友元函数不是类的成员函数
void printObj(const Student *p)
{
	std::cout << "名字 : " << p->m_name << std::endl;
	std::cout << "年龄 : " << p->m_age << std::endl;
}

void Student::showAddress(Address &addr)
{
	std::cout << "名字 : " << m_name << std::endl;
	std::cout << "年龄 : " << m_age << std::endl;
	std::cout << "住址 : " << addr.m_provinces << "省" << addr.m_city << "市" << addr.m_region << "县" << std::endl;
}

void Student::showAddress1(Address1 &addr)
{
	std::cout << "名字 : " << m_name << std::endl;
	std::cout << "年龄 : " << m_age << std::endl;
	std::cout << "住址 : " << addr.m_provinces << "省" << addr.m_city << "市" << addr.m_region << "县" << std::endl;
}