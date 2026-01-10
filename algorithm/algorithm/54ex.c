/*
	xyz, yzz 是一个三位数, xyz + yzz = 532, 求 x y z 的值
*/
#include <stdio.h>
int main()
{
	int x, y, z;

	for (int i = 100; i <= 999; i++) {
		x = i / 100;  // 百位
		y = i / 10 % 10;  // 十位
		z = i % 10;  // 个位

		if (i + (y * 100 + z * 10 + z) == 532) {
			printf("%d %d %d %d", i, x, y, z);
		}
	}
	return 0;
}