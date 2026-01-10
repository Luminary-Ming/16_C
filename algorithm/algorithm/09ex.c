/*
	个位数为6且能被3整除但不能被5整除的三位自然数共有多少个，分别是哪些？
*/
#include <stdio.h>

int main()
{
	int count = 0;

	// 遍历所有三位数 100 - 999
	for (int i = 100; i <= 999; i++) {
		if ((i % 10 == 6) && (i % 3 ==0) && (i % 5 != 0)) {
			count++;
			printf("%d\n", i);
		}
	}
	printf("共有 %d 个\n", count);
	return 0;
}