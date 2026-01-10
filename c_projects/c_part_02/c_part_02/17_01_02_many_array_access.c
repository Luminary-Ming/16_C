/*
    数组的访问  ->   数组名 + 偏移量
*/
#include <stdio.h>

int main(void)
{
    int arr[3][4] = { 1,2,3,4,5,6,7,8,9,10,11,12 };
    int i = 0, j = 0;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            //printf("arr[%d][%d] = %d\n", i, j, arr[i][j]);
            printf("*(*(arr + %d)) = %d\n", i, j, *(*(arr + i) + j));
        }
    }

    return 0;
}
