#include <stdio.h>

int main(void)
{
    char i = 13;  // char 类型的变量, 数据占 1 个字节
    int *p = &i;  // int *类型的指针访问数据时, 是以 4 个字节进行访问空间大小的

    printf("i = %d\n", i);  // i = 13
    printf("&i = %p\n", &i);  // &i = 0x7fffffffe8ef
    printf("p = %p\n", p);  // p = 0x7fffffffe8ef
    printf("*p = %d\n", *p);  // *p = -1511667  ->  垃圾值

    return 0;
}
/*
    00001101  -> 0x1000 
    ????????
    ????????
    ????????

    *p 在访问是以 0x1000 为起始 4 个字节长度的储存空间
    访问超过了 i 范围的 3 个相邻字节, 这些字节内容都是未初始化的垃圾值
*/