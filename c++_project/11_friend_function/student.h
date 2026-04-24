#ifndef STUDENT_H
#define STUDENT_H

// 前置声明
class Address;
class Address1;

// 学生类
class Student
{
	// 访问修饰符
private:  // 公有访问权限: 以下成员可以在类外部被访问
	// 定义成员变量 -> 类的属性
	int m_age;  // m_ 前缀表示 member (成员)
	const char *m_name;

	// 静态的成员变量, 统计有多少个学生对象
	// 无论调用哪种构造函数, 都进行计数变量自增
	static int sum;  

public:
	// 定义成员函数 -> 类的行为
	//Student(void);  // 无参构造函数 (编译器默认生成)

	Student(int age = 23, const char *name = "2233娘")  // 全参构造函数 (带默认参数, 会与无参构造冲突)
	{
		m_age = age;
		m_name = name;
		sum++;
	}

/*
	Student(const char *name, int age)  // 重载构造函数, 参数顺序不同
	{
		m_name = name;
		m_age = age;
	}
*/

	// 构造函数也可以使用 初始化列表 的方式来初始化成员变量 (仅适用于构造函数, 普通函数不行)
	// 等价于上面的构造函数 "Student(const char *name, int age) { ... }"
	Student(const char *name, int age) : m_name(name), m_age(age) { sum++; }

	// 浅拷贝构造函数 (编译器默认生成)
	Student(const Student &s)
	{
		m_name = s.m_name;
		m_age = s.m_age;
		sum++;
	}

	void show();  // 成员函数声明, 用于输出学生信息
	void setName(const char *name);
	void setAge(int age);

	// get 方法只需要获取成员变量的值, 不需要改变成员变量的值, 所有可以用 const 关键字修饰
	// const 修饰的成员函数是常函数, 对于成员变量是只读的
	// 在 C++ 中这样去写
	int getAge() const;
	const char *getName() const;  // 前面的 const 修饰的是返回值, 后面的 const 修饰成员变量只读

	// 静态成员函数只能访问静态成员变量, 不能访问其他成员变量
	static int getSum();

	// 声明友元函数 (友元函数不是类的成员函数, 只是需要声明在类内部, 本质还是普通函数)
	friend void printObj(const Student *p);  

	void showAddress(Address &addr);

	void showAddress1(Address1 &addr);
};

// 普通函数 (定义在类外, 非成员函数)
// 成员变量只能由成员函数访问
// 所以普通函数不能访问成员变量
// 可通过 friend 关键字解决
//void printObj(const Student *p);


// 地址类
class Address
{
public:
	// 构造函数 (使用初始化列表的方式初始化成员变量)
	Address(const char *p, const char *c, const char *r) : m_provinces(p), m_city(c), m_region(r) {};

	// 声明友元函数, 这样 Student 类的对象就能访问 Address 类中的所有成员变量了 (包括私有成员变量)
	// 记得把 Address 类定义在 Student 类前面
	friend void Student::showAddress(Address &addr);

private:
	const char *m_provinces;  // 省份
	const char *m_city;       // 城市
	const char *m_region;     // 区域
};


// 地址类
class Address1
{
public:
	// 构造函数 (使用初始化列表的方式初始化成员变量)
	Address1(const char *p, const char *c, const char *r) : m_provinces(p), m_city(c), m_region(r) {};

	// 声明有元类, 这样 Student 类就能使用 Address1 类中所有的成员函数了
	// 记得把 Address1 类定义在 Student 类前面
	friend class Student;

private:
	const char *m_provinces;  // 省份
	const char *m_city;       // 城市
	const char *m_region;     // 区域
};

#endif