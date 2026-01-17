/*
    项目中调试模式和发布模式
*/
#include <stdio.h>

#define DEBUG  // 定义 DEBUG 宏

int main(void)
{
#ifdef DEBUG
    int a = 10;
    printf("调试模式 : a = %d\n", a);
#else
    int a = 20;
    printf("发布模式 : a = %d\n", a);
#endif

    return 0;
}