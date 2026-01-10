/*
	用 for 编程找出100~200中的完全平方数
*/
#include <stdio.h>
#include <math.h>

int main()
{
	// 遍历 100 - 200
	for (int i = 100; i <= 200; i++) {

		int root = (int)sqrt(i);

		if (root * root == i) {
			printf("%d = %d^2\n", i, root);
		}
	}
	return 0;
}