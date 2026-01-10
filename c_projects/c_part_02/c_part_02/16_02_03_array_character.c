/*
    数组的特性  -> 一维数组的数组名特性
*/
#include <stdio.h>

int main(void)
{
    int arr[5];

    // &arr[0]  表示数组首成员地址
    printf("&arr[0] = %p\n", &arr[0]);

    // arr  表示数组首成员地址
    printf("arr = %p\n", arr);

    // &arr  表示整个数组的地址
    printf("&arr = %p\n", &arr);

    printf("----------------------------\n");
    printf("&arr[0] + 1 = %p\n", &arr[0] + 1);
    printf("arr + 1 = %p\n", arr + 1);
    printf("&arr + 1 = %p\n", &arr + 1);

    return 0;
}
/*
    &arr[0] = 0x7fffffffe8e0
    arr = 0x7fffffffe8e0
    &arr = 0x7fffffffe8e0
    ----------------------------
    &arr[0] + 1 = 0x7fffffffe8e4
    arr + 1 = 0x7fffffffe8e4
    &arr + 1 = 0x7fffffffe8f4
*/