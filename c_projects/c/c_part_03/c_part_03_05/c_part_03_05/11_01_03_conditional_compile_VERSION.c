/*
    项目中发行版本
*/
#include <stdio.h>

#define VERSION 2  // 定义 VERSION 宏

int main(void)
{

#if VERSION==1
    printf("版本1 : 基本功能!\n");
#elif VERSION==2
    printf("版本2 : 新增付费功能!\n");
#else
    printf("未知版本");
#endif

    return 0;
}