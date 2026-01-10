/*
    数组和函数
*/
#include <stdio.h>

void print(int *p, int size);
void change(int p[5], int size);

void print(int *p, int size)
{
    int i = 0;

    for (i = 0; i < size; i++)
        printf("p[%d] = %d\n", i, p[i]);
}

void change(int p[5], int size)
{
    int i = 0;

    for (i = 0; i < size; i++)
        scanf("%d", p + i);
}

int main(void)
{
    int arr[5] = { 11,22,33,44,55 };
    int size = sizeof(arr) / sizeof(arr[0]);

    print(arr, size);  // 通过 print() 打印数组成员的数据

    change(arr, size);  // 通过 change() 改变数组成员的数据

    print(arr, size);  // 通过 print() 打印数组成员的数据

    return 0;
}