#include <stdio.h>
/*
	计算1+2+3+…+n的值，n 是从键盘输入的自然数。
*/
int main()
{
	int n, sum = 0;
	printf("请输入一个自然数: ");
	scanf("%d", &n);

	// 检测输入是否为自然数
	if (n < 1) {
		printf("输入错误! 请输入一个自然数(大于等于1的整数)\n");

		return 1;  // 有错误, 退出程序
	}

	// 计算1到n的和
	for (int i = 1; i <= n; i++) {
		sum += i;  // sum = sum + i
	}

	printf("1到%d的和是: %d\n", n, sum);

	return 0;
}