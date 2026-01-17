/*
    二维数组的特性  ->  二维数组的数组名的特性
*/
#include <stdio.h>

int main(void)
{
    int arr[3][4];

    // &arr[0][0] 表示数组首成员的地址
    printf("&arr[0][0] = %p\n", &arr[0][0]);

    // &arr[0] 表示数组首行的地址
    printf("&arr[0] = %p\n", &arr[0]);

    // arr[0] 表示数组首行首成员的地址
    printf("arr[0] = %p\n", arr[0]);

    // arr 表示数组首行的地址
    printf("arr = %p\n", arr);

    // *arr 表示数组首行首成员的地址
    printf("*arr = %p\n", *arr);

    // &arr 表示整个数组的地址
    printf("&arr = %p\n", &arr);

    printf("-------------------------------\n");
    printf("&arr[0][0] + 1 = %p\n", &arr[0][0] + 1);
    printf("&arr[0] + 1 = %p\n", &arr[0] + 1);
    printf("arr[0] + 1 = %p\n", arr[0] + 1);
    printf("arr + 1 = %p\n", arr + 1);
    printf("*arr + 1 = %p\n", *arr + 1);
    printf("&arr + 1 = %p\n", &arr + 1);

    return 0;
}
/*
    &arr[0][0] = 0x7fffffffe8c0
    &arr[0] = 0x7fffffffe8c0
    arr[0] = 0x7fffffffe8c0
    arr = 0x7fffffffe8c0
    *arr = 0x7fffffffe8c0
    &arr = 0x7fffffffe8c0
    -------------------------------
    &arr[0][0] + 1 = 0x7fffffffe8c4
    &arr[0] + 1 = 0x7fffffffe8d0
    arr[0] + 1 = 0x7fffffffe8c4
    arr + 1 = 0x7fffffffe8d0
    *arr + 1 = 0x7fffffffe8c4
    &arr + 1 = 0x7fffffffe8f0
*/