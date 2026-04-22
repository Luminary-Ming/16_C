#include <iostream>
#include "student.h"

// 类成员函数的实现 (类名::函数名)
void Student::show(void)
{
	std::cout << m_name << "年龄: " << m_age << std::endl;
}