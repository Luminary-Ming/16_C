/*
	标准格式化 - 字符和字符串
*/
#include <stdio.h>

int main(void)
{
	printf("%c\n", 'A');  // A
	printf("%s\n", "easthome");  // easthome

	// 只有字符和整数类型可以相互转换, 依赖于 ASCII 码表
	printf("%d\n", 'A');  // 65
	printf("%c\n", 65);  // A
	return 0;
}
/*
	`%c`   单一字符
	`%s`   字符串
*/