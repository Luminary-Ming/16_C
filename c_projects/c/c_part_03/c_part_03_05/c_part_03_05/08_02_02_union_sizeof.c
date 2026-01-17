/*
    共用体所占的字节大小以最大成员的数据类型为准
*/
#include <stdio.h>

struct S
{
    char c;
    short s;
    int i;
    long l;
    float f;
    double d;
};

union U
{
    char c;
    short s;
    int i;
    long l;  // 8
    float f;
    double d;  // 8
};

int main(void)
{
    printf("sizeof(struct S) = %ld\n", sizeof(struct S));  // 32
    printf("sizeof(union U) = %ld\n", sizeof(union U));    // 8

    return 0;
}
