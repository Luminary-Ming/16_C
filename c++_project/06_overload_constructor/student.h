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

	Student(int age = 23, const char *name = "2233娘")  // 全参构造函数 (带默认参数, 会与无参构造冲突)
	{
		m_age = age;
		m_name = name;
	}


	//Student(int age, const char *name)  // 重载构造函数, 参数顺序不同
	//{
	//	m_name = name;
	//	m_age = age;
	//}


	// 构造函数也可以使用 初始化列表 的方式来初始化成员变量 (仅适用于构造函数, 普通函数不行, 普通函数没有初始化列表)
	// 等价于上面的构造函数 "Student(const char *name, int age) { ... }"
	Student(const char *name, int age) : m_name(name), m_age(age) {}

	// 浅拷贝构造函数 (编译器默认生成)
	Student(const Student &s)
	{
		m_name = s.m_name;
		m_age = s.m_age;
	}

	void show();  // 成员函数声明, 用于输出学生信息
	void setName(const char *name);
	void setAge(int age);
};

#endif