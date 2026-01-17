/*
    共用体的起始地址都是相同的
*/
#include <stdio.h>

union U
{
    char c;
    short s;
    int i;
    long l;
    float f;
    double d;
};

int main(void)
{
    union U u;

    printf("&u = %p\n", &u);      // 0x7ffe2ca54d08
    printf("&u.c = %p\n", &u.c);  // 0x7ffe2ca54d08
    printf("&u.s = %p\n", &u.s);  // 0x7ffe2ca54d08
    printf("&u.i = %p\n", &u.i);  // 0x7ffe2ca54d08
    printf("&u.l = %p\n", &u.l);  // 0x7ffe2ca54d08
    printf("&u.f = %p\n", &u.f);  // 0x7ffe2ca54d08
    printf("&u.d = %p\n", &u.d);  // 0x7ffe2ca54d08

    return 0;
}