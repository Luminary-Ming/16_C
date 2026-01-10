/*
    二级指针的运算
*/
#include <stdio.h>

int main(void)
{
    int i = 9527;
    int *p = &i;
    int **pp = &p;

    printf("i = %d\n", i);  // i = 9527
    printf("*p = %d\n", *p);  // *p = 9527
    printf("**pp = %d\n", **pp);  // **pp = 9527
    printf("--------------------\n");
    printf("&i = %p\n", &i);  // &i = 0x7fffffffe8c4
    printf("p = %p\n", p);  // p = 0x7fffffffe8c4
    printf("*pp = %p\n", *pp);  //  *pp = 0x7fffffffe8c4
    printf("--------------------\n");
    printf("&p = %p\n", &p);  // &p = 0x7fffffffe8c8
    printf("pp = %p\n", pp);  // pp = 0x7fffffffe8c8
    printf("--------------------\n");
    printf("&pp = %p\n", &pp);  //  &pp = 0x7fffffffe8d0

    return 0;
}