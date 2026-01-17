/*
    数组的访问  ->  数组名 + 偏移量
*/
#include <stdio.h>

int main(void)
{
	int arr[5];
	int i = 0;

	//int arr_size = sizeof(arr) / sizeof(arr[0]);
	int arr_size = sizeof(arr) / sizeof(*arr);

    for (i = 0; i < arr_size; i++){
        //scanf("%d", &arr[i]);
        scanf("%d", arr + i);
    }

    for (i = 0; i < arr_size; i++){
        //printf("arr[%d] = %d\n", i, arr[i]);
        printf("*(arr + %d) = %d\n", i, *(arr + i));
    }
       
    return 0;
}