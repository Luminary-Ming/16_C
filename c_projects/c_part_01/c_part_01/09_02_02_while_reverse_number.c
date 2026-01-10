/*
	while 循环

	输入一个整数, 输出它的反序数
	如 : 12345 -> 54321
*/
#include <stdio.h>

int main(void)
{
	int n = 0;
	scanf("%d", &n);

	int res = 0;
	while (n > 0) {
		res = n % 10 + res * 10;
		n /= 10;
	}

	printf("%d", res);
	return 0;
}