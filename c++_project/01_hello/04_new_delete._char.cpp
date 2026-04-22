#include <iostream>

int main(void)
{
	char *s = new char[32];  // 开辟动态存储空间 ( 相当于 char *s = malloc(sizeof(char) * 32) )
	if (s == NULL)
	{
		std::cout << "Dynamic Memory Is Failed!" << std::endl;
		return -1;
	}

	std::cin >> s;  // 通过 cin 给指针 s 指向的动态存储空间赋值, 最多输入 31 个字符

	std::cout << "s = " << s << std::endl;  // 对于 char * 类型它被特殊对待为字符串, 所以会打印字符串而不是地址

	delete[] s;  // 释放动态存储空间, 相当于 free(s);
	s = NULL;  // 避免使用野指针

	return 0;
}

/*
	xinming@01_hello$ g++ 04_new_delete._char.cpp

	xinming@01_hello$ ./a.out

	nihao
	s = nihao

	xinming@01_hello$
*/