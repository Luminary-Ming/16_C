/*
    continue 语句

    continue 只能出现在循环体中
    continue 在循环体中是跳出本次循环, 进入下一次循环
    continue 在嵌套循环体中只能跳出 continue 所在的循环体中
*/

#include <stdio.h>

int main(void)
{
    int i = 0;

    for (i = 0; i < 5; i++)
    {
        if (i == 2) continue;
        printf("i = %d\n", i);
    }

    return 0;
}
/*
    i = 0
    i = 1
    i = 3
    i = 4
*/