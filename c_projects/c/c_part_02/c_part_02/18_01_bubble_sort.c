/*
	冒泡排序
	录入 10 个数, 从小到大排序
*/
#include <stdio.h>

#define ARR_LENGTH 10

void init_arr(int *arr, int size);  // 初始化数组
void sort(int *arr, int size);  // 排序
void print_arr(int *arr, int size);

int main(void) 
{
	int arr[ARR_LENGTH] = { 0 };

	init_arr(arr, ARR_LENGTH);
	
	sort(arr, ARR_LENGTH);

	print_arr(arr, ARR_LENGTH);
}

void init_arr(int *arr, int size)
{
	printf("录入 10 个数 :\n");
	for (int i = 0; i < size; i++)
	{
		scanf("%d", arr + i);
	}
}

void sort(int *arr, int size)
{
	// 轮数, n 个数需要 n-1 轮 (因为最后一轮只剩 1 个数，不用比了)
	for (int i = 0; i < size - 1; i++)
	{
		// 每轮比较的次数
		// 第 1 轮  ->  比较 n-1-0 次
		// 第 2 轮  ->  比较 n-1-1 次
		// 第 i 轮  ->  比较 n-1-i 次
		for (int j = 0; j < size - 1 - i; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

void print_arr(int *arr, int size)
{
	printf("排序后 :\n");
	for (int i = 0; i < size; i++)
	{
		printf("%d ", arr[i]);
	}
}