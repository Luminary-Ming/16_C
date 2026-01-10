/*
    数组的访问  ->  数组名 + 下标
*/
#include <stdio.h>

int main(void)
{
    int arr[5];

    arr[0] = 9527;
    arr[1] = 12345;
    arr[2] = 100;
    arr[3] = -50;
    arr[4] = 11;

    printf("arr[0] = %d\n", arr[0]);
    printf("arr[1] = %d\n", arr[1]);
    printf("arr[2] = %d\n", arr[2]);
    printf("arr[3] = %d\n", arr[3]);
    printf("arr[4] = %d\n", arr[4]);

    return 0;
}
/*
    arr[0] = 9527
    arr[1] = 12345
    arr[2] = 100
    arr[3] = -50
    arr[4] = 11
*/