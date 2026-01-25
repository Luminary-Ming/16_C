#include <stdio.h>
#include "queue.h"

#define QUEUESIZE 6
int queue_arr[QUEUESIZE];  // 队列的存储空间
int queue_front;  // 对头的标记
int queue_rear;  // 队尾的标记

static int is_full(void)  // 队列是满的
{
	return ((queue_rear + 1) % QUEUESIZE) == queue_front;
}

static int is_empty(void)  // 队列是空的
{
	return queue_rear == queue_front;
}

int queue_en(int data)  // 入队
{
	if (is_full())
		return -1;
	queue_rear = (queue_rear + 1) % QUEUESIZE;  // 移动队尾标记
	queue_arr[queue_rear] = data;  // 入队
	return 0;
}

int queue_de(int *save)
{
	if (is_empty())
		return -1;
	queue_front = (queue_front + 1) % QUEUESIZE;  // 移动队尾标记
	*save = queue_arr[queue_front];  // 入队
	return 0;
}

void queue_display(void)
{
	if (is_empty())
		return;  // 队列为空, 直接返回

	int i = 0;  // 使用 i 代替 front 做操作
	i = (queue_front + 1) % QUEUESIZE;

	while (i != queue_rear)
	{
		printf("%d\n", queue_arr[i]);
		i = (i + 1) % QUEUESIZE;
	}
	printf("%d\n", queue_arr[i]);  // 打印 queue_front 和 queue_rear 相遇的位置
}