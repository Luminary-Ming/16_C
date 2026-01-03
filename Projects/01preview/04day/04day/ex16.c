/*
	求 n! ( n 的 阶乘 )

	0! = 1, 1! = 1
*/
#include <stdio.h>

int main()
{
	int n;
	scanf("%d", &n);

	int factorial = 1;

	for (int i = n; i >= 1; i--) {
		factorial *= i ;
	}

	printf("%d! = %d\n", n, factorial);
	return 0;
}