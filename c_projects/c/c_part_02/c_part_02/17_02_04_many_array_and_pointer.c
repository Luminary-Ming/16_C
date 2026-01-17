#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int arr[3][4];
    int *p;
    int **pp;

    p = arr;   // 错误：类型不匹配
    pp = arr;  // 错误：类型不匹配

    int (*p1)[4] = arr;  // 指向包含 4 个 int 数组的指针

    // 二级指针的正确用法（模拟二维数组）
    int **pp = malloc(3 * sizeof(int *));  // 分配指针数组
    for (int i = 0; i < 3; i++)
    {
        pp[i] = malloc(4 * sizeof(int));  // 每个指针指向一个int数组
    }

    return 0;
}
/*
    int arr[3][4]; -> arr 的类型是 int (*)[4] 指向包含 4 个 int 类型数组的指针
    不是 int * (指向 int 的指针)
    也不是 int** (指向 int 指针的指针)
*/