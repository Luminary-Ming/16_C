#include "student.h"
#include <iostream>

int main(void)
{
	Student stu1;  // 在栈上创建一个 Student 类型的对象 stu1
	Student stu2;  // 在栈上创建一个 Student 类型的对象 stu2
	Student stu3("22娘", 22);  // 调用构造函数初始化实例对象
	Student stu4(stu3);  // stu4 浅拷贝 stu3

	// 通过 stu1 对象访问成员变量
	stu1.setName("玲玲");
	stu1.setAge(18);
	stu1.show();  // 调用对象 stu1 的 show 成员函数, 输出 "玲玲年龄: 18"

	stu2.show();  // 使用全参构造中的默认值
	stu3.show();
	stu4.show();

	std::cout << "一共有 " << Student::getSum() << " 个学生" << std::endl;


	std::cout << stu3.getName() << ", " << stu3.getAge() << std::endl;

	const Student stu5("33娘", 33);  // const 修饰的对象是常对象, 常对象只能访问常函数

	std::cout << stu5.getName() << ", " << stu5.getAge() << std::endl;

	
	printObj(&stu5);  // 调用友元函数


	Address addr("河北", "沧州", "南皮");
	stu4.showAddress(addr);


	Address1 addr1("河北", "沧州", "泊头");
	stu3.showAddress1(addr1);

	return 0;
}
/*
	玲玲年龄: 18
	2233娘年龄: 23
	22娘年龄: 22
	22娘年龄: 22
	一共有 4 个学生
	22娘, 22
	33娘, 33
	名字 : 33娘
	年龄 : 33
	名字 : 22娘
	年龄 : 22
	住址 : 河北省沧州市南皮县
	名字 : 22娘
	年龄 : 22
	住址 : 河北省沧州市泊头县
*/
