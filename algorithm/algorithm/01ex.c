#include <stdio.h>
/*
	输入一个不超过五位的正整数，输出其逆数。例如输入12345，输出应为54321。
*/
int main()
{
	int num, reversed = 0;
	printf("请输入一个不超过五位的正整数: ");
	scanf("%d", &num);

	// 检测输入是否超过五位
	if (num <= 0 || num > 99999) {
		printf("输入错误! 请输入1-99999之间的正整数\n");
		return 1;
	}

	printf("原数: %d\n", num);

	// 反转数字
	int temp = num;
	while (temp > 0) {
		reversed = temp % 10 + reversed * 10;
		temp = temp / 10;
	}

	printf("逆数: %d\n", reversed);

	return 0;
}