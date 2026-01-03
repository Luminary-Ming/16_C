/*
	输入 n , n 是直角三角形的高, 打印直角三角形
	例如: n = 8
	*
	**
	***
	****
	*****
	******
	*******
	********
	比如循环到第三行( i = 3 )有三个 * ( j = i ), 5个空格( k = n - i )
*/
#include <stdio.h>

int main()
{
	int n;
	scanf("%d", &n);

	// 行 
	for (int i = 1; i <= n; i++) {
		// 打印 * 号
		for (int j = 1; j <= i; j++) {
			printf("*");
		}
		// 打印空格
		for (int k = 1; k <= n - i; k++) {
			printf(" ");
		}
		printf("\n");
	}
	
	return 0;
}