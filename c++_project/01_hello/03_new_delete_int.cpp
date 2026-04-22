#include <iostream>

int main(void)
{
	int *p = new int;  // 开辟动态存储空间 ( 相当于 int *p = malloc(sizeof(int)) )
	if (p == NULL)  // 开辟失败
	{
		std::cout << "Dynamic Memory Is Failed!" << std::endl;
		return -1;
	}

	*p = 2233;  // 通过指针 p 访问指向的动态存储空间

	std::cout << "[1]p = " << p << " *p = " << *p << std::endl;

	std::cin >> *p;  // 通过 cin 给指针 p 指向的动态存储空间赋值

	std::cout << "[2]p = " << p << " *p = " << *p << std::endl;

	delete p;  // 释放动态存储空间, 相当于 free(p);
	p = NULL;  // 避免使用野指针

	return 0;
}
/*
	xinming@01_hello$ g++ 03_new_delete_int.cpp

	xinming@01_hello$ ./a.out

	[1]p = 0x5f3bd0202eb0 *p = 2233
	114511
	[2]p = 0x5f3bd0202eb0 *p = 114511

	xinming@01_hello$
*/