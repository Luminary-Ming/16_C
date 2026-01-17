#include <stdio.h>
/*
	scanf() 函数用于从标准输入(键盘)
	1. scanf() 双引号中只写标准化格式,不写提示语句
	2. scanf() 双引号中不要写换行符
	3. scanf() 双引号后要的是一个地址, 变量名前要加 & 符号(取地址符)

	scanf() 需要知道 "往哪里写"，& 就是告诉它 "写在这个地址"。
*/
int main(void)
{
	int year = 0, month = 0, day = 0;

	scanf("%d-%d-%d", &year, &month, &day);

	// 输入 2025-12-24
	printf("%d年%d月%d日\n", year, month, day);  // 输出 : 2025年12月24日

	return 0;
}