/*
    数组的访问  ->  指针 + 下标
*/
#include <stdio.h>

int main(void)
{
    int arr[5];
    int size = sizeof(arr) / sizeof(arr[0]);
    int* p = arr;
    int i = 0;

    for (i = 0; i < size; i++)
        //scanf("%d", &arr[i]);
        //scanf("%d", arr + i);
        //scanr("%d", p + i);
        scanf("%d", &p[i]);

    for (i = 0; i < size; i++)
        //printf("arr[%d] = %d\n", i, arr[i]);
        //printf("*(arr + %d) = %d\n", i, *(arr + i));
        //printf("*(p + %d) = %d\n", i, *(p + i));
        printf("p[%d] = %d\n", i, p[i]);

    return 0;
}