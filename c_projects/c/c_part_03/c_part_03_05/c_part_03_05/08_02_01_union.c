/*
    共用体变量访问成员的时候, 只有最后一次存储数据的成员有效 (覆盖前面的成员)
*/
#include <stdio.h>

union U
{
    int i;
    float f;
};

int main(void)
{
    union U u;

    u.i = 100;
    u.f = 3.14;  // 最后一次存储数据的成员有效

    printf("u.f = %f\n", u.f);  // 3.14
    printf("u.i = %d\n", u.i);  // 1078523331 (3.14 转为二进制后再用十进制读出来的结果)

    return 0;
}