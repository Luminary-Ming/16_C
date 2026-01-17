#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *arr = malloc(10 * sizeof(int));  //  开辟 10 个连续的 int 类型的空间
    if (arr == NULL)  
    {
        fprintf(stderr, "malloc() Is Failed!\n");  // 打印错误信息
        return -1;  // 开辟失败
    }

    for (int i = 0; i < 10; i++)
    {
        arr[i] = i * 10;  // 赋值
    }

    // realloc()  重新调整开辟的空间 (扩容或缩小)
    int *new_arr = realloc(arr, 5 * sizeof(int));  // 将原来 10 个连续的 int 类型的空间调整为 5 个连续的 int 类型的空间 (缩小)
    //int *new_arr = realloc(arr, 20 * sizeof(int));  // 将原来 10 个连续的 int 类型的空间调整为 20 个连续的 int 类型的空间 (扩容)
    if (new_arr == NULL)
    {
        fprintf(stderr, "realloc() Is Failed!\n");
        free(arr);  // 释放原空间
        return -2;
    }

    arr = new_arr;  // 新地址赋给 arr

    for (int i = 0; i < 5; i++)
    {
        arr[i] = i + 10;
    }

    for (int i = 0; i < 5; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("\n");

    free(arr);  // 释放开辟的空间(因为 arr 和 new_arr 首地址相同, 所以释放一个就行)
    arr = new_arr = NULL;  // 释放完全都赋空

    return 0;
}