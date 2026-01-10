/*
    指针的使用 - 偏移量
*/
#include <stdio.h>

int main(void)
{
    int i = 13;
    int *p = &i;

    printf("i = %d\n", i);   // i = 13
    printf("&i = %p\n", &i);  // &i = 0x7fffffffe8ec
    printf("p = %p\n", p);  // p = 0x7fffffffe8ec
    printf("*p = %d\n", *p);  // *p = 13

    p++;

    printf("i = %d\n", i);  // i = 13
    printf("&i = %p\n", &i);  // &i = 0x7fffffffe8ec
    printf("p = %p\n", p);  // p = 0x7fffffffe8f0
    printf("*p = %d\n", *p);  // *p = -16

    return 0;
}
/*
    p = 0x7fffffffe8ec
    p++ = p + sizeof(int) = 0x7fffffffe8ec + 4 = 0x7fffffffe8f0
*/