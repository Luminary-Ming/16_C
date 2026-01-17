/*
	宏函数
	实现打印两个数之间的最大值
*/
#include <stdio.h>

#define MAX(A,B) (A) > (B) ? (A) : (B)

int main(void)
{
	printf("MAX = %d\n", MAX(5,6));
	return 0;
}