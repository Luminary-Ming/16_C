#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include "person.h"

// Worker 类以 public 方式继承 Person 类
class Worker : public Person
{
private:
	float m_salary;  // 薪资

public:
	Worker(const char *name, int age, float salary);  // 构造函数
	void show() const;  // 打印信息
};

#endif