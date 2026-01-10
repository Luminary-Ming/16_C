/*
	for 循环

	求所有的水仙花数
	三位数的水仙花数有 153 370 371 407
*/
#include <stdio.h>

int main(void)
{
	int a = 0, b = 0, c = 0;
	int i = 0;
	int count = 0; // 计算数量

	for (i = 100; i < 1000; i++) {
		a = (i / 100);  // 百位
		b = (i / 10 % 10);  // 十位
		c = (i % 10);  // 个位
		if (a * a * a + b * b * b + c * c * c == i) {
			printf("[%d] %d 是水仙花数\n", ++count, i);
		}
	}


	return 0;
}