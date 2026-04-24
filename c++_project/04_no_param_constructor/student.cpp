#include <iostream>
#include "student.h"

Student::Student(void)
{
	std::cout << "Student() Is Called !" << std::endl;
}

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