/*
    条件运算符 ( 三目运算符 )
*/
#include <stdio.h>

int main(void)
{
    int a = 0;
    scanf("%d", &a);

    (a % 2) == 0 ? printf("%d是一个偶数!\n", a) : printf("%d是一个奇数!\n", a);

    return 0;
}