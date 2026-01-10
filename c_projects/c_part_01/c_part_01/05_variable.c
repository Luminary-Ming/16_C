/*
    变量
*/
#include <stdio.h>

int main(void)
{
    char c = -128;
    char a = -200;

    printf("c = %d\n", c);  // c = -128
    printf("c = %p\n", c);  // c = 0xffffff80

    printf("a = %d %p\n", a, a);  // a = 56 0x38

    return 0;
}
/*
    char 类型数据范围 [-127 ~ 128] 或 [0 ~ 255] 

    signed char [-127 ~ 128]
    -128(原) 1000 0000  最高位即是符号位, 也是数据位

    unsigned char [0 ~ 255]
     255(原) 1111 1111  


    -200 超出了 signed char [-127 ~ 128] 的范围

    short 类型的 -200
    -200(原) 1000 0000 1100 1000
    -200(补) 1111 1111 0011 1000
    存入 char
    溢出截断, 只保留低 8 位
    变成               0011 1000
    十进制             56
*/