#include "person.h"
#include "student.h"
#include "worker.h"

int main(void)
{
	Person p1("22娘", 22);
	Worker w1("33娘", 33, 2233);

	Person *p = &p1;
	p->show();

	p = &w1;  // 多态
	p->show();

	return 0;
}

/*
	Person  类的构造函数被调用了 !
	Person  类的构造函数被调用了 !
	姓名: 22娘 年龄: 22
	工人姓名: 33娘, 年龄: 33, 薪水: 2233
	Person  类的析构函数被调用了 !
	Person  类的析构函数被调用了 !
*/