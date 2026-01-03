#include <stdio.h>
/*
    关系运算符
    表达式经过关系运算符运算后会生成一个真假值
    0 为假, 非0为真
    默认真值是 1 , 但不一定是 1 , 只要非0都表示真
*/
int main()
{
    int a = 0, b = 0;

    scanf("%d %d", &a, &b);

    printf("%d > %d -> %d\n", a, b, a > b);  // 7 > 5 -> 1
    printf("%d < %d -> %d\n", a, b, a < b);  // 7 < 5 -> 0
    printf("%d == %d -> %d\n", a, b, a == b);  // 7 == 5 -> 0
    printf("%d >= %d -> %d\n", a, b, a >= b);  // 7 >= 5 -> 1
    printf("%d <= %d -> %d\n", a, b, a <= b);  // 7 <= 5 -> 0
    printf("%d != %d -> %d\n", a, b, a != b);  // 7 != 5 -> 1

    return 0;
}