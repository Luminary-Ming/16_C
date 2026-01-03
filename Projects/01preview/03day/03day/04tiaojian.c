#include <stdio.h>
/*
    条件运算符 / 三目运算符
*/
int main()
{
    int a = 0, b = 0;
    int max = 0;  //max变量存储a和b中的最大值

    scanf("%d %d", &a, &b);

    max = a > b ? a : b;

    printf("max = %d\n", max);

    return 0;
}