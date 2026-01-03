#include <stdio.h>
/*
	键入一个整数x，按如下规则计算并输出结果：
	如果小于 3，则结果为 x + 3；
	如果大于等于 3 ，则结果为 3x + 9；
	如果大于 9，则结果为 9x - 10。
*/
int main()
{
	int x = 0;
	scanf("%d", &x);

	if (x < 3) {
		printf("结果为: %d", x + 3);
	}
	else if (x >= 3 && x <= 9) {
		printf("结果为: %d", 3 * x + 9);
	}
	else if (x > 9) {
		printf("结果为: %d", 9 * x - 10);
	}

	return 0;
}
