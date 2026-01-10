/*
    break 语句

    break 只能用在 switch 语句和循环语句中
    break 在循环语句, 功能是结束循环, 跳出循环语句
    break 在嵌套循环语句, 只能跳出最近的循环体
*/

#include <stdio.h>

int main(void)
{
    int i = 0;

    for (i = 0; i < 5; i++)
    {
        if (i == 2) break;
        printf("i = %d\n", i);
    }

    return 0;
}
/*
    i = 0
    i = 1
*/