#include "person.h"
#include "student.h"

int main(void)
{
	Student s1("22娘", 22, 98);
	Student s2("33娘", 33, 99);

	s1.print();
	s2.print();

	s1.show();
	s2.show();

	return 0;
}

//  可以看出
//  构造函数 的调用顺序是先调用父类的构造函数, 再调用子类的构造函数
//  析构函数 的调用顺序是先调用子类的析构函数, 再调用父类的析构函数
/*
	Person  类的构造函数被调用了 !
	Student 类的构造函数被调用了 ! 
	Person  类的构造函数被调用了 !
	Student 类的构造函数被调用了 !
	姓名: 22娘 年龄: 22
	成绩是: 98
	姓名: 33娘 年龄: 33
	成绩是: 99
	姓名: 22娘 年龄: 22
	成绩是: 98
	姓名: 33娘 年龄: 33
	成绩是: 99
	Student 类的析构函数被调用了 !
	Person  类的析构函数被调用了 !
	Student 类的析构函数被调用了 !
	Person  类的析构函数被调用了 !

*/