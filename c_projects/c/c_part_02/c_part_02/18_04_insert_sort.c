/*
	插入排序
	将需要排序的数列分为已排序和未排序两个部分
	每一次从未排序的部分中取一个元素, 插入到已排序的正确的位置上
	类似于打扑克抓牌再整理手中的牌的过程
*/
#include <stdio.h>

#define ARRSIZE 5

void insert_sort(int *arr, int size);  // 插入排序

void print(const int *arr, int size);  // 打印数组

int main(void)
{
	int arr[ARRSIZE] = { 201, 100, 70, 50, 30 };

	print(arr, ARRSIZE);  // 遍历数组

	insert_sort(arr, ARRSIZE);  // 插入排序

	print(arr, ARRSIZE);

	return 0;
}

void insert_sort(int *arr, int size)
{
	if (size <= 1) return;  // 不需要排序, 直接结束

	int i = 0, j = 0;
	int save = 0;  // 用于临时存储待插入的数据

	for (i = 1; i < size; i++) // 外层循环, 从第 2 个元素开始, 循环到 size - 1
	{
		save = arr[i];  // 保存等待插入的元素数据
		j = i - 1;  // 指向当前元素的前一个位置, 也是已排序区最后一个位置
		while (j >= 0 && arr[j] > save)  // 内层循环, 将已排序的部分和 save 作比较, 大的往后移
			// j >= 0 防止数组越界(防止是负的) 
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = save;  // 循环结束后, j+1 的位置就是 save 要插入的位置
		print(arr, size);  // 为了调试查看插入排序的过程
	}

}

void print(const int *arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
	getchar();
}