#include "student.h"

int main(void)
{
	Student stu1;  // 在栈上创建一个 Student 类型的对象 stu1
	Student stu2;  // 在栈上创建一个 Student 类型的对象 stu2

	stu2.show();  // 使用全参构造中的默认值

	// 通过 stu1 对象访问成员变量
	stu1.setName("玲玲");
	stu1.setAge(18);

	stu1.show();  // 调用对象 stu1 的 show 成员函数, 输出 "玲玲年龄: 18"

	return 0;
}
/*
	2233娘年龄: 23
	玲玲年龄: 18
*/