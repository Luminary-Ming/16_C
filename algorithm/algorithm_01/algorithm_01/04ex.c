#include <stdio.h>

/*
	键盘输入 5 个整数到数组a中，然后将 a 逆序复制到数组 b 中，并输出 b 中各元素的值
*/

int main()
{
	const int SIZE = 5;  // 定义数组大小

	int a[SIZE], b[SIZE];  // 定义数组

	int i = 0;  // 循环变量
	for (i = 0; i < SIZE; i++) {
		scanf("%d", &a[i]);  // 输入5个整数到数组a中
	}

	// 逆序复制
	for (i = 0; i < SIZE; i++) {
		b[i] = a[SIZE - 1 - i];
	}

	// 打印 b 数组中的各元素
	for (i = 0; i < SIZE; i++) {
		printf("b[%d] = %d\n", i, b[i]);
	}

	return 0;
}