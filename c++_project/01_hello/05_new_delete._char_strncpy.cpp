#include <iostream>
#include <cstring>

int main(void)
{
	char *s = new char[32];  // 开辟动态存储空间 ( 相当于 char *s = malloc(sizeof(char) * 32) )
	if (s == NULL)
	{
		std::cout << "Dynamic Memory Is Failed!" << std::endl;
		return -1;
	}

	//s = "xinming";  // 语法没问题, 但是意义变了, 并不是把字符串赋值到动态存储空间; 让指针指向了另一块内存 (字符串字面量), 从而丢失了对原内存的引用

	std::strncpy(s, "xinming", std::strlen("xinming"));

	std::puts(s);  // 打印输出

	delete[] s;  // 释放动态存储空间, 相当于 free(s);
	s = NULL;  // 避免使用野指针

	return 0;
}

/*
	xinming@01_hello$ g++ 05_new_delete._char_strncpy.cpp

	xinming@01_hello$ ./a.out

	xinming

	xinming@01_hello$
*/