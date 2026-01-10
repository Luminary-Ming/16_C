/*
    数组的特性  -> 通过 sizeof() 运算符查看数组的特性
*/
#include <stdio.h>

int main(void)
{
    int arr[5];
    int i = 0;  // 循环变量

    int arr_size = sizeof(arr) / sizeof(arr[0]);  // 数组长度

    printf("sizeof(arr) = %ld\n", sizeof(arr));
    printf("sizeof(arr[0]) = %ld\n", sizeof(arr[0]));
    printf("sizeof(arr) / sizeof(arr[0]) = %ld\n", sizeof(arr) / sizeof(arr[0]));

    for (i = 0; i < arr_size; i++) {
        printf("sizeof(arr[%d]) = %ld\n", i, sizeof(arr[i]));
    }

    return 0;
}
/*
    sizeof(arr[0]) = 4
    sizeof(arr) / sizeof(arr[0]) = 5
    sizeof(arr[0]) = 4
    sizeof(arr[1]) = 4
    sizeof(arr[2]) = 4
    sizeof(arr[3]) = 4
    sizeof(arr[4]) = 4
*/