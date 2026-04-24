#ifndef PERSON_H
#define PERSON_H

class Person
{
public:
	Person(const char *name, int age);  // 构造函数
	// 多态的必要条件: 基类函数用 virtual 关键字修饰变成虚函数
	// 基类函数必须加 virtual, 派生类函数可加可不加 (因为编译器会自动给派生类的同名函数添加 virtual 关键字修饰)
	// 并且派生类必须对虚函数进行重写
	// 加上 virtual 后, C++ 就会对该函数采用 "动态绑定", 动态绑定的核心是: 在运行时根据对象的实际类型来决定调用哪个函数
	virtual void show() const;  // 打印信息 (不需要修改成员变量, 所以可以加 const, 声明为常函数)
	~Person();  // 析构函数

protected:
	const char *m_name;
	int m_age;
};

#endif