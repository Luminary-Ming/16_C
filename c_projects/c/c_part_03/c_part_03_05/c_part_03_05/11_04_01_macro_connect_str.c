/*
    `\`  反斜杠, 续行符, 将下一行连接到当前行, 把代码连接成一行
*/
#include <stdio.h>

int main(void)
{
    int a = 2, b = 3, c = 4;

    // '\' 反斜杠, 将下一行连接到当前行, 把代码连接成一行
    if ((a + b > c) && \
        (a + c > b) && \
        (b + c > a))
    {
        printf("可以构成普通三角形!\n");
    }

    return 0;
}