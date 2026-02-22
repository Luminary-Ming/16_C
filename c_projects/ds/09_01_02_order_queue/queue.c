#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

QUEUE *queue_create(void)
{
	QUEUE *queue = malloc(sizeof(QUEUE));
	if (queue == NULL)
		return NULL;

	queue->queue_front = queue->queue_rear = 0;  // 初始化对头队尾标记

	return queue;
}

static int is_full(QUEUE *queue)  // 队列是满的
{
	return (queue->queue_rear + 1) % QUEUESIZE == queue->queue_front;
}

static int is_empty(QUEUE *queue)  // 队列是空的
{
	return queue->queue_rear == queue->queue_front;
}

// 入队
int queue_en(QUEUE *queue, int data)
{
	if (is_full(queue)) return -1;

	queue->queue_rear = (queue->queue_rear + 1) % QUEUESIZE;  // 移动队尾标记
	queue->queue_arr[queue->queue_rear] = data;

	return 0;
}

// 出队
int queue_de(QUEUE *queue, int *save)
{
	if (is_empty(queue)) return -1;

	queue->queue_front = (queue->queue_front + 1) % QUEUESIZE;  // 移动队尾标记
	*save = queue->queue_arr[queue->queue_front];

	return 0;
}

// 遍历 
void queue_display(QUEUE *queue)
{
	if (is_empty(queue)) return;

	int i = (queue->queue_front + 1) % QUEUESIZE;  // 使用 i 代替 front 做操作
	while (i != queue->queue_rear)
	{
		printf("%d\n", queue->queue_arr[i]);
		i = (i + 1) % QUEUESIZE;
	}
	printf("%d\n", queue->queue_arr[i]);  // 打印 queue_front 和 queue_rear 相遇的位置
}
