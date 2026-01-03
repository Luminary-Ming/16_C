#include <stdio.h>
/*
	标准化格式
	标准化格式并不是字符, 而是一种格式
	该格式由 % 开头, 后面跟一个或多个字符组成
	功能是告诉 printf() 函数如何去打印对应的数据
	单独打印 % 打印不出来是因为 % 是占位符
*/
int main()
{
	int num = 123;
	// %d  打印十进制整数
	printf("%d\n", num);  // 123
	// %c  打印字符 'A'
	printf("%c\n", 'A');  // A
	// %f  打印浮点数(默认小数点后 6 位)
	printf("%f\n", 3.14);  // 3.140000
	// %s  打印字符串
	printf("%s\n", "Hello World!");  // Hello World!
	// %o  打印 123 的八进制
	printf("%o\n", num);  // 173
	// %x  打印 123 的十六进制
	printf("%x\n", num);  // 7b
	// %p  打印变量num的内存地址  
	// & 符号的作用：取地址运算符
	printf("%p\n", &num); // 0000004E0D4FF5B4 
	// %%  打印百分号 %
	printf("%%\n");  // %

	// 只有字符和整数类型可以相互转换, 依赖于 ASCII 码表
	printf("%d\n", 'A');  // 65
	printf("%c\n", 65);  // A

	return 0;
}