#include <stdio.h>
#include "queue.h"
#include <stdlib.h>

QUEUE *queue_create(void)
{
	QUEUE *q = malloc(sizeof(QUEUE));
	if (q == NULL)
		return NULL;
	q->queue_front = q->queue_rear = 0;  // 初始化对头队尾标记
	return q;
}

static int is_full(QUEUE *q)  // 队列是满的
{
	return ((q->queue_rear + 1) % QUEUESIZE) == q->queue_front;
}

static int is_empty(QUEUE *q)  // 队列是空的
{
	return q->queue_rear == q->queue_front;
}

int queue_en(QUEUE *q, int data)  // 入队
{
	if (is_full(q))
		return -1;
	q->queue_rear = (q->queue_rear + 1) % QUEUESIZE;  // 移动队尾标记
	q->queue_arr[q->queue_rear] = data;  // 入队
	return 0;
}

int queue_de(QUEUE *q, int *save)
{
	if (is_empty(q))
		return -1;
	q->queue_front = (q->queue_front + 1) % QUEUESIZE;  // 移动队尾标记
	*save = q->queue_arr[q->queue_front];  // 入队
	return 0;
}

void queue_display(QUEUE *q)
{
	if (is_empty(q))
		return;  // 队列为空, 直接返回

	int i = 0;  // 使用 i 代替 front 做操作
	i = (q->queue_front + 1) % QUEUESIZE;

	while (i != q->queue_rear)
	{
		printf("%d\n", q->queue_arr[i]);
		i = (i + 1) % QUEUESIZE;
	}
	printf("%d\n", q->queue_arr[i]);  // 打印 queue_front 和 queue_rear 相遇的位置
}


void queue_destroy(QUEUE *q)
{
	if (is_empty(q))
		return;  // 队列为空, 直接返回
	free(q);
}