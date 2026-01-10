/*
	标准格式化 - 特殊类型
*/
#include <stdio.h>

int main(void)
{
    int a = 100;

    // 第二个 %p 错误地使用：传递了整数 a , 这会输出 a 的值作为地址
    printf("a = %d %p %p\n", a, &a, a);  // a = 100 0x7fffffffe8f4 0x64

    // "Hello World!\n" 共 13 个字符
    // 所以 %n 会将 13 写入 &a 指向的内存位置（即变量 a）
    // a 的值从 100 变为 13
    printf("Hello World!\n%n", &a);  // Hello World!

    printf("a = %d %p %p\n", a, &a, a);  // a = 13 0x7fffffffe8f4 0xd

    printf("%%");  // %

    return 0;
}
/*
	`%p`   地址 (也可以输出变量在内存中怎么存的 )
    `%n`   写入已输出的字符数 ( 不包括尾零 )
    `%%`   百分号本身
*/