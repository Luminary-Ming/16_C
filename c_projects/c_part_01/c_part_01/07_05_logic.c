#include <stdio.h>
/*
	逻辑运算符
	表达式经过逻辑运算符的运算后会生成一个真假值

	输入一个年份, 判断是否为闰年并输出结果
*/
int main()
{
	int year = 0;  // 定义变量存储年份+++
	scanf("%d", &year);

	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
		printf("%d 是闰年\n", year);  // 输出闰年信息
	}
	else {
		printf("%d 是平年\n", year);  // 输出平年信息
	}

	return 0;
}