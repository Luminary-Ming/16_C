#ifndef STUDENT_H
#define STUDENT_H

#include "person.h"  

// Student 类以 public 的方式继承 Person 类
class Student : public Person
{
public:
	// Student 类是 Person 类的派生类, 拥有 Person 类的成员变量和成员函数
	// 但是 Person 类中的 m_name 和 m_age 都是 private 修饰, 所以派生类中不可见
	// 但能通过调用基类的构造函数进行初始化
	Student(const char *name, int age, float score);  // 构造函数
	void print() const;  // 打印信息
	void show() const;  // 打印信息 (基类和派生类函数重名情况)
	~Student();  // 析构函数

private:
	float m_score;  // 成绩
};

#endif