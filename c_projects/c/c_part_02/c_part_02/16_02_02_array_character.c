/*
    数组的特性  -> 数组成员的地址

    数组成员的地址是连续的
    所以可以通过某个成员的地址进行加减法运算, 进行地址偏移, 到找到其他成员的位置
*/
#include <stdio.h>

int main(void)
{
    int arr[5];
    int arr_size = sizeof(arr) / sizeof(arr[0]);
    int i = 0;  // 循环变量

    for (i = 0; i < arr_size; i++)
        printf("&arr[%d] = %p\n", i, &arr[i]);
        
    return 0;
}
/*
    &arr[1] = 0x7fffffffe8e4
    &arr[2] = 0x7fffffffe8e8
    &arr[3] = 0x7fffffffe8ec
    &arr[4] = 0x7fffffffe8f0
*/