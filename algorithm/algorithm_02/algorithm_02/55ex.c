/*
	计算 1的平方 + 2的平方 + 3的平方 + ... + 10的平方的结果
*/
#include <stdio.h>
int main()
{
	int factorial, sum = 0;

	for (int i = 1; i <= 10; i++) {
		factorial = i * i;
		sum += factorial;
	}
	printf("%d", sum);
	return 0;
}