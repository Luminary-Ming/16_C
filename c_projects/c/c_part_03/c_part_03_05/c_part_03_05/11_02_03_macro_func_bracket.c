/*
	宏函数中参数为什么要加括号 ?
*/
#include <stdio.h>

#define MAX(A,B) A > B ? A : B

int max(int a, int b)
{
    return a > b ? a : b;
}

int main(void)
{
    int a = 13, b =7;
	printf("MAX = %d, a = %d, b = %d\n", MAX(a++, b++), a, b);  // MAX = 14, a = 15, b = 8  

	a = 13, b = 7;
	printf("max = %d, a = %d, b = %d\n", max(a++, b++), a, b);  // max = 13, a = 14, b = 8
	return 0;
}
/*
	如果没有括号, 宏就是替换
	MAX(a++, b++) 替换为 a++ > b++ ? a++ : b++
	==> a++ > b++, 先用后自增, 13 > 7 为真, a 变成 14, b 变成 8, 返回 a++
	==> a++ 先用后自增, 返回 a(14), 然后自增 a 变成 15, b 还是 8
*/