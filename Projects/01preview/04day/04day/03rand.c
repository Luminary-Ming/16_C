#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a = 0;  //循环变量
    int i = 0;  //存储获取到的随机数

    for (a = 0; a < 5; a++)
    {
        i = rand() % 1000;  // 0 - 999
        i = rand() % 90 + 10;  // 10 - 99
        i = rand() % 100 - 50;  // -50 - 49
        printf("i = %d\n", i);
    }

    return 0;
}