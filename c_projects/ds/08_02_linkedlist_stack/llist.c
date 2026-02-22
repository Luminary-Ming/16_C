#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "llist.h"

LLIST *llist_create(void)
{
	LLIST *handler = malloc(sizeof(LLIST));
	if (!handler)
	{
		perror("malloc failed");
		return NULL;
	}

	handler->prev = handler->next = handler;
	return handler;
}

int llist_insert(LLIST *handler, const void *data, int mode)
{
	LLIST *newnode = malloc(sizeof(LLIST));
	if (!newnode)
	{
		perror("malloc failed");
		return -1;
	}

	memcpy(newnode, data, sizeof(LLIST));

	LLIST *p = handler;
	switch (mode)
	{
		case HEADINSERT: break;
		case TAILINSERT: p = p->prev; break;
		default: free(newnode); return -2;
	}

	newnode->prev = p;
	newnode->next = p->next;
	newnode->prev->next = newnode;
	newnode->next->prev = newnode;

	return 0;
}

void llist_display(LLIST *handler)
{
	LLIST *cur = handler->next;
	while (cur != handler)
	{
		printf("%8d\n", cur->id);
		cur = cur->next;
	}
}

void llist_destory(LLIST *handler)
{
	if (handler == NULL) return;

	LLIST *cur = handler->next;
	while (cur != handler)
	{
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		free(cur);
		cur = handler->next;
	}

	free(handler);
}

int llist_is_empty(LLIST *handler)
{
	return handler->next == handler;
}

// 删除(Pop) : 删除头结点之后的第一个节点
int llist_pop(LLIST *handler, void *save)
{
	LLIST *cur = handler->next;

	if (llist_is_empty(handler)) return -1;

	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;

	memcpy(save, &cur->id, sizeof(cur->id));

	free(cur);

	return 0;
}
