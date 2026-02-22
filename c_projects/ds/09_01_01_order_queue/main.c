/*
	顺序队列 - 版本一
*/
#include <stdio.h>
#include "queue.h"

int main(void)
{
	int arr[] = { 11,22,33,44,55,66,77,88,99 };  // 数据源
	
	int ret = 0;
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		ret = queue_en(arr[i]);  // 入队
		if (ret != 0)
		{
			printf("Queue Is Full!\n");
			break;
		}
	}

	queue_display();

	printf("========================\n");

	int save = 0;
	ret = queue_de(&save);
	if (ret != 0)
		printf("Queue Is Empty!\n");
	else
		printf("save = %d \n", save);

	printf("========================\n");

	queue_display(); 

	printf("========================\n");

	queue_en(9527);

	queue_display(); 

	return 0;
}