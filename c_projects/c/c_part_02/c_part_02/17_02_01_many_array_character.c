/*
	二维数组的特性  ->  通过 sizeof() 运算符进行计算
*/
#include <stdio.h>

int main(void)
{
	int arr[3][4];

	printf("sizeof(arr[0][0]) = %ld\n", sizeof(arr[0][0]));
	printf("sizeof(arr[0]) = %ld\n", sizeof(arr[0]));
	printf("sizeof(arr) = %ld\n", sizeof(arr));
	printf("sizeof(arr) / sizeof(arr[0][0]) = %ld\n", sizeof(arr) / sizeof(arr[0][0]));
	printf("sizeof(arr[0]) / sizeof(arr[0][0]) = %ld\n", sizeof(arr[0]) / sizeof(arr[0][0]));

	return 0;
}
/*
	sizeof(arr[0][0]) = 4
	sizeof(arr[0]) = 16
	sizeof(arr) = 48
	sizeof(arr) / sizeof(arr[0][0]) = 12
	sizeof(arr[0]) / sizeof(arr[0][0]) = 4
*/