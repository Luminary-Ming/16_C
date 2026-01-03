/*
	输入三个整数 x, y, z, 把这三个数从小到大输出
*/
#include <stdio.h>

int main()
{
	int x, y, z;
	int temp;
	printf("请输入三个整数：");
	scanf("%d %d %d", &x, &y, &z);

	// 如果 x > y, 交换位置
	if (x > y) {
		temp = x;
		x = y;
		y = temp;
	}

	if (y > z) {
		temp = y;
		y = z;
		z = temp;
	}

	if (x > y) {
		temp = x;
		x = y;
		y = temp;
	}

	printf("从小到大：%d %d %d\n", x, y, z);

	return 0;
}