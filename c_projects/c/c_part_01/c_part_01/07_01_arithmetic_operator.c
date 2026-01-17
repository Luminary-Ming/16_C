#include <stdio.h>
/*
	算术运算符
*/
int main(void)
{
	int a = 13, b = 7;

	printf("%d + %d = %d\n", a, b, a + b);  // 13 + 7 = 20
	printf("%d - %d = %d\n", a, b, a - b);  // 13 - 7 = 6
	printf("%d * %d = %d\n", a, b, a * b);  // 13 * 7 = 91
	printf("%d / %d = %d\n", a, b, a / b);  // 13 / 7 = 1
	printf("%d %% %d = %d\n", a, b, a % b);  // 13 % 7 = 6

	return 0;
}

/*
	交换两个变量的值

	char a = 13, b = 7;

	// 第一种
	char temp = a;  // temp = 13
	a = b;  // a = 7
	b = temp;  // b = 13

	// 第二种
	a = a + b;  // 13 + 7 = 19
	b = a - b;  // 19 - 7 = 13
	a = a - b;  // 19 - 13 = 7

	// 第三种
	a = a ^ b;  // a = 0000 1101 ^ 0000 0111 = 0000 1010
	b = a ^ b;  // b = 0000 1010 ^ 0000 0111 = 0000 1101 = 13
	a = a ^ b;  // a = 0000 1010 ^ 0000 1101 = 0000 0111 = 7

	-----------------------------------------------------------

	多位数取出每一位

	int num = 9527;

	printf("千位 : %d\n", num / 1000);
	printf("百位 : %d\n", num / 100 % 10);4
	printf("十位 : %d\n", num / 10 % 10);
	printf("个位 : %d\n", num % 10);
*/