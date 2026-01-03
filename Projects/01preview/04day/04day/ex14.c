/*
	猴子吃桃问题:
	一天一只猴子摘了一些桃子, 它吃了一半又吃了一个,
	第二天也是吃了一半又吃了一个,
	到了第十天吃完只剩一个桃子了,
	问猴子摘了多少桃子
*/
#include <stdio.h>

int main() {
	int peach = 1; // 第10天吃完后剩的桃子

	// 从第9天倒推到第1天
	for (int day = 9; day >= 1; day--) {
		peach = (peach + 1) * 2;
	}

	printf("猴子第一天摘了 %d 个桃子\n", peach);
	return 0;
}