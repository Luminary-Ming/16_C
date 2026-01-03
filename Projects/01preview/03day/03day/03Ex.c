#include <stdio.h>
/*
	输入一个四位数的正整数, 判断该数是否为回文数
*/
int main()
{
	int num = 0;
	scanf("%d", &num);

	if (num < 0 || num > 9999) {
		printf("输入错误！请输入0到9999之间的四位正整数。\n");
		return 1;
	}

	// 判断是否为回文数 : 个位 = 千位， 十位 = 百位
	if ((num % 10 == num / 1000) && (num / 10 % 10 == num / 100 % 10)) {
		printf("%d是回文数。\n", num);
	}
	else {
		printf("%d不是回文数。\n", num);
	}

	return 0;
}