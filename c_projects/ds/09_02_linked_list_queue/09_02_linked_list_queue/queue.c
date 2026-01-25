#include <stdio.h>
#include "queue.h"
#include <stdlib.h>
#include "llist.h"

QUEUE *queue_create(void)
{
	return llist_create();
}

static int is_full(QUEUE *q)  
{
	return 0;  // 链式队列不会满
}

static int is_empty(QUEUE *q)  // 队列是空的
{
	return llist_is_empty(q);
}

int queue_en(QUEUE *q, int data)  // 入队
{
	if (is_full(q))
		return -1;
	
	llist_insert(q, &data, TAILINSERT);
	return 0;
}

int queue_de(QUEUE *q, int *save)
{
	if (is_empty(q))
		return -1;
	llist_pop(q, save);
	return 0;
}

void queue_display(QUEUE *q)
{
	if (is_empty(q))
		return;  // 队列为空, 直接返回

	llist_display(q);
}

void queue_destroy(QUEUE *q)
{
	if (is_empty(q))
		return;  // 队列为空, 直接返回
	llist_destroy(q);
}