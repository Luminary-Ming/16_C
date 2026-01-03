#include <stdio.h>
/*
    sizeof( 变量名/数据类型 ) 运算符, 计算变量或数据类型所占用的字节数
    1 byte = 8 bit
    1 word = 2 / 4 byte
    1 halfword = 1 / 2 byte
*/
int main()
{
    printf("sizeof(char) = %ld\n", sizeof(char));  // sizeof(char) = 1
    printf("sizeof(short) = %ld\n", sizeof(short));  // sizeof(short) = 2
    printf("sizeof(int) = %ld\n", sizeof(int));  // sizeof(int) = 4
    printf("sizeof(long) = %ld\n", sizeof(long));  // sizeof(long) = 8  linux 64位中就是8, windows 中是4, 原因是 windows 兼容32位
    printf("sizeof(float) = %ld\n", sizeof(float));  // sizeof(float) = 4
    printf("sizeof(double) = %ld\n", sizeof(double));  // sizeof(double) = 8

    return 0;
}
