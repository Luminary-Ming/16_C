/*
	选择排序
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
		// 1. 假设当前元素是最小的
		int min_index = i;

		// 2. 在剩下的元素中找出真正的最小值
		for (int j = i + 1; j < size; j++)
		{
			if (arr[j] < arr[min_index])
			{
				min_index = j;  // 更新最小值的索引
			}
		}

		// 3. 如果找到了更小的值, 交换
		if (min_index != i)
		{
			int temp = arr[i];
			arr[i] = arr[min_index];
			arr[min_index] = temp;
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