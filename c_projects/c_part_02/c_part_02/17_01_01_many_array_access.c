/*
    数组的访问  ->  数组 + 下标
*/
#include <stdio.h>

int main(void)
{
    int arr[3][4];
    int i = 0, j = 0;
    int num = 100;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            arr[i][j] = num;
            num++;
        }
    }

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("arr[%d][%d] = %d\n", i, j, arr[i][j]);
        }
    }

    return 0;
}