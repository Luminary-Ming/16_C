#include <stdio.h>
/*
	算术运算符
*/
int main()
{
	int a = 13, b = 7;

	printf("%d + %d = %d\n", a, b, a + b);  // 13 + 7 = 20
	printf("%d - %d = %d\n", a, b, a - b);  // 13 - 7 = 6
	printf("%d * %d = %d\n", a, b, a * b);  // 13 * 7 = 91
	printf("%d / %d = %d\n", a, b, a / b);  // 13 / 7 = 1
	printf("%d %% %d = %d\n", a, b, a % b);  // 13 % 7 = 6

	return 0;
}