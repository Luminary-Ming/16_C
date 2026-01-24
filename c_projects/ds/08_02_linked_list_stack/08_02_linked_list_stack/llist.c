#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


LLIST *llist_create(void)
{
	LLIST *handler = NULL;  // handler 指针指向开辟的头节点
	handler = malloc(sizeof(LLIST));  // 开辟头节点

	handler->prev = handler->next = handler;
	return handler;
}

int llist_insert(LLIST *handler, const void *data, int mode)
{
	LLIST *newnode = malloc(sizeof(LLIST));
	if (newnode == NULL)
		return -1;
	memcpy(newnode, data, sizeof(LLIST));

	LLIST *p = handler;  // p 代替 handler 做操作
	switch (mode)
	{
		case HEADINSERT:break;
		case TAILINSERT:p = p->prev; break;
		default:free(newnode); return -2;
	}
	newnode->next = p->next;
	newnode->prev = p;
	newnode->next->prev = newnode;
	newnode->prev->next = newnode;
	return 0;
}

void llist_display(LLIST *handler)
{
	LLIST *cur = NULL;
	for (cur = handler->next; cur != handler; cur = cur->next)
		printf("%d\n", cur->id);
}

void llist_destroy(LLIST *handler)
{
	LLIST *cur = NULL;
	for (cur = handler->next; cur != handler; cur = handler->next)
	{
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
		free(cur);
	}
	free(cur);
}

int llist_is_empty(LLIST *handler)
{
	return handler->next == handler;
}

int llist_pop(LLIST *handler, void *save)
{
	LLIST *cur = handler->next;

	if (llist_is_empty(handler))
		return -1;
	cur->next->prev = cur->prev;  // 摘除节点
	cur->prev->next = cur->next;
	memcpy(save,&cur->id,sizeof(cur->id));
	free(cur);

	return 0;
}