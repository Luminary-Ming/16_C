#ifndef PERSON_H
#define PERSON_H

class Person
{
public:
	Person(const char *name, int age);  // 构造函数
	void show() const;  // 打印信息 (不需要修改成员变量, 所以可以加 const, 声明为常函数)
	~Person();  // 析构函数

private:
	const char *m_name;
	int m_age;
};

#endif