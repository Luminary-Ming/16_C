#include <stdio.h>
#include "add.h"

int main(void)
{
	int a = 0, b = 0;
	int sum = 0;
	scanf("%d %d", &a, &b);

	sum = add(a, b);  // 功能函数的调用

	printf("sum = %d\n", sum);
}