#include "llist.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>


QUEUE *queue_create(void)
{
	return llist_create();
}

static int is_full(QUEUE *queue)
{
	return 0;  // 链式队列不会满
}

static int is_empty(QUEUE *queue)
{
	return llist_is_empty(queue);
}

int queue_en(QUEUE *q, int data)
{
	if (is_full(q)) return -1;

	llist_insert(q, &data, TAILINSERT);

	return 0;
}

int queue_de(QUEUE *queue, int *save)
{
	if (is_empty(queue)) return -1;

	llist_de(queue, save);

	return 0;
}

void queue_display(QUEUE *queue)
{
	if (is_empty(queue)) return;

	llist_display(queue);
}

void queue_destroy(QUEUE *queue)
{
	if (is_empty(queue)) return;

	llist_destroy(queue);
}