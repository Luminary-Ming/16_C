#include "student.h"

int main(void)
{
	Student stu;  // 在栈上创建一个 Student 类型的对象 stu
	
	// 通过 stu 对象访问成员变量
	stu.m_name = "玲玲";
	stu.m_age = 18;

	stu.show();  // 调用对象 stu 的 show 成员函数, 输出 "玲玲年龄: 18"


	Student *p;  // 声明一个指向 Student 类型对象的指针 p
	p = &stu;  // 将指针 p 指向对象 stu 的地址

	// 通过指针 p 访问成员变量
	p->m_age = 20;
	p->m_name = "欣慰";

	p->show();  // 通过指针 p 调用所指向对象的 show 成员函数, 输出 "欣慰年龄: 20"

	return 0;
}
/*
	玲玲年龄: 18
	欣慰年龄: 20
*/