#include <stdio.h>
/*
    sizeof () 运算符  计算数据类型占用内存的字节大小
*/
int main(void)
{
    printf("%ld\n", sizeof(short));  // 2
    printf("%ld\n", sizeof(int));   // 4
    printf("%ld\n", sizeof(long));  // 8
    printf("%ld\n", sizeof(char));  // 1
    printf("%ld\n", sizeof(float));  // 4
    printf("%ld\n", sizeof(double));  // 8
    printf("%ld\n", sizeof(long long));  // 8
    printf("%ld\n", sizeof(long double));  // 16
    printf("%d\n", sizeof(size_t));  // 8  


    int a = 0;
    int byte = 0;  // 接收 sizeof() 运算的结果
    byte = sizeof(a = 9527);
    printf("a = %d byte = %d\n", a, byte);  // a = 0 byte = 4

    return 0;
}
/*
    如果 sizeof() 中写的是一个表达式, 表达式的内容不会计算
    因为 sizeof() 运算符内部只会求变量或数据类型的字节大小, 所以内部的表达式不会计算
*/