/*
    二级指针的加减法运算
    二级指针的偏移量都是相同的, 都是 8 字节
*/
#include <stdio.h>

void print(int *p)
{
    printf("*p = %d\n", *p);
}

void change(int **p, int *q)
{
    *p = q;
}

int main(void)
{
    int a = 13, b = 7;
    int *p = &a;

    print(p);  // *p = 13

    change(&p, &b);  // 改变 p 指针指向, 改为指向变量 b

    print(p);  // *p = 7

    return 0;
}