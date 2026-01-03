/*
	打印 i * j 的 * 边框
	如: i = 4, j = 5
	*****
	*   *
	*   *
	*****
*/
#include <stdio.h>
int main()
{
	int i = 0, j = 0;
	scanf("%d %d", &i, &j);

	// i 行
	for (int x = 1; x <= i; x++) {
		// j 列
		for (int y = 1; y <= j; y++) {
			// 第二行 到 倒数第二行
			if (x > 1 && x < i) {
				// 如果是第一列和最后一列打印 * 号, 其他列打印空格
				if (y == 1 || y == j) {
					printf("*");
				}
				else {
					printf(" ");
				}	
			}
			else {
				printf("*");
			}

		}
		printf("\n");
	}

	return 0;
}