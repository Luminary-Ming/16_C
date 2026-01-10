#include <stdio.h>
#include <math.h>
/*
	求n以内（不包括n）同时能被3和7整除的所有自然数之和的平方根s，n从键盘输入。
	例如若n为1000时，函数值应为：s=153.909064 ( 6 位小数 )。
*/
int main()
{
	int n = 0, sum = 0;
	scanf("%d", &n);

	int i = 0;
	for (i = 0; i < n; i++) {
		if (i % 3 == 0 && i % 7 == 0) {
			sum += i;
		}
	}


	printf("平方根为: %f\n", sqrt(sum));
	return 0;
}