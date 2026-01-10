#include <stdio.h>
/*
    VT 控制码
    打印输出时给内容添加一些属性, 如: 颜色, 粗体, 下划线, 背景色等
    \33[0m  关闭所有属性
    \33[3m	斜体
    \33[4m	下划线
    \33[5m	闪烁
    ...
*/
int main(void)
{
    printf("\33[30m\33[47mHello World!\33[0m\n");
    printf("\33[31m\33[46mHello World!\33[0m\n");
    printf("\33[32m\33[45mHello World!\33[0m\n");
    printf("\33[33m\33[44mHello World!\33[0m\n");
    printf("\33[34m\33[43mHello World!\33[0m\n");
    printf("\33[35m\33[42mHello World!\33[0m\n");
    printf("\33[36m\33[41mHello World!\33[0m\n");
    printf("\33[37m\33[40mHello World!\33[0m\n");
    return 0;
}