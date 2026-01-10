/*
    数组的访问  ->  下标 + 数组名 / 指针
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
        //scanf("%d", arr+i);
        //scanf("%d", p+i);
        //scanf("%d", &p[i]);
        //scanf("%d", &i[arr]);
        scanf("%d", &i[p]);

    for (i = 0; i < size; i++)
        //printf("arr[%d] = %d\n", i, arr[i]);
        //printf("*(arr + %d) = %d\n", i, *(arr + i));
        //printf("*(p + %d) = %d\n", i, *(p + i));
        //printf("p[%d] = %d\n", i, p[i]);
        //printf("%d[p] = %d\n", i, i[arr]);
        printf("%d[p] = %d\n", i, i[p]);

    return 0;
}
