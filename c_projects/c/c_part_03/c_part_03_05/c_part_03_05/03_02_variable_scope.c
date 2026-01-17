/*
    变量的作用域
*/
#include <stdio.h>

int main(void)
{
    int i = 10;     // 外层 i = 10
    printf("外层 i = %d\n", i);

    if (1)
    {
        int i = 20; // 内层 i = 20，隐藏了外层 i
        printf("内层 i = %d\n", i);  // 输出 20
    }

    printf("外层 i = %d\n", i);      // 输出 10，内层变量已销毁

    return 0;
}