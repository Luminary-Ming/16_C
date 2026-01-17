/*
    二维数组的特性  ->  二维数组成员的地址
*/
#include <stdio.h>

int main(void)
{
    int arr[3][4];
    int i = 0, j = 0;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("&arr[%d][%d] = %p\n", i, j, &arr[i][j]);
        }
    }

    return 0;
}
/*
    &arr[0][1] = 0x7fffffffe8c4
    &arr[0][2] = 0x7fffffffe8c8
    &arr[0][3] = 0x7fffffffe8cc
    &arr[1][0] = 0x7fffffffe8d0
    &arr[1][1] = 0x7fffffffe8d4
    &arr[1][2] = 0x7fffffffe8d8
    &arr[1][3] = 0x7fffffffe8dc
    &arr[2][0] = 0x7fffffffe8e0
    &arr[2][1] = 0x7fffffffe8e4
    &arr[2][2] = 0x7fffffffe8e8
    &arr[2][3] = 0x7fffffffe8ec
*/