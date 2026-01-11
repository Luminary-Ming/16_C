/*
	4 5 6 7 总共能组成多少个不被 4 整除的 4 位数
*/

#include <stdio.h>

int main()
{
	int count = 0;  // 符合的个数

	for (int a = 4; a <= 7; a++) {
		for (int b = 4; b <= 7; b++) {
			if (b == a) continue;  // 数字不重复

			for (int c = 4; c <= 7; c++) {
				if (c == a || c == b) continue;  // 数字不重复

				for (int d = 4; d <= 7; d++) {
					if (d == a || d == b || d == c) continue;  // 数字不重复

					// 不被 4 整除
					int num = a * 1000 + b * 100 + c * 10 + d;
					if (num % 4 != 0) {
						count++;
						printf("%d\n", num);
					}
				}
			}

		}
	}

	printf("\n\n总共：%d 个\n", count);
	return 0;
}