#include <stdio.h>
/*
	定义一个变量, 录入一个四位数的正整数, 分别取出千位, 百位, 十位, 个位并输出
*/
int main()
{
	int num = 0, one = 0, ten = 0, hundred = 0, thousandth = 0;
	scanf("%d", &num);

	thousandth = num / 1000;
	hundred = num / 100 % 10;
	ten = num / 10 % 10;
	one = num % 10;

	printf("千位 : %d\n", thousandth);
	printf("百位 : %d\n", hundred);
	printf("十位 : %d\n", ten);
	printf("个位 : %d\n", one);

	return 0;
}