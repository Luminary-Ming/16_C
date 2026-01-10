/*
    关系运算符

    一个表达式经过关系运算符运算, 结果是一个真假值

    0  为假
    非 0  为真 ( 默认真值是 1 )
*/

#include <stdio.h>

int main(void)
{
    int a = 13, b = 7;

    printf("%d > %d\t%d\n", a, b, a > b);  // 13 > 7  1
    printf("%d < %d\t%d\n", a, b, a < b);  // 13 < 7  0
    printf("%d == %d\t%d\n", a, b, a == b);  // 13 == 7 0
    printf("%d >= %d\t%d\n", a, b, a >= b);  // 13 >= 7 1
    printf("%d <= %d\t%d\n", a, b, a <= b);  // 13 <= 7 0
    printf("%d != %d\t%d\n", a, b, a != b);  // 13 != 7 1

    return 0;
}