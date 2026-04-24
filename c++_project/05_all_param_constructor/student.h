#ifndef STUDENT_H
#define STUDENT_H

// 学生类
class Student
{
	// 访问修饰符
private:  // 公有访问权限: 以下成员可以在类外部被访问
	// 定义成员变量 -> 类的属性
	int m_age;  // m_ 前缀表示 member (成员)
	const char *m_name;

public:
	// 定义成员函数 -> 类的行为
	//Student(void);  // 无参构造函数 (编译器默认生成)
	Student(int age = 23, const char *name = "2233娘");  // 全参构造函数 (带默认参数, 会与无参构造冲突)
	void show();  // 成员函数声明, 用于输出学生信息
	void setName(const char *name);
	void setAge(int age);
};

#endif