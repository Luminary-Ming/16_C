#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LLIST *llist_create(void)
{
	LLIST *handler = malloc(sizeof(LLIST));
	if (handler == NULL)
		return NULL;

	handler->prev = handler->next = handler;
	return handler;
}

int llist_insert(LLIST *handler, const void *data, int mode)
{
	LLIST *newnode = malloc(sizeof(LLIST));
	if (newnode == NULL) return -1;

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
		printf("%d\n", cur->id);
		cur = cur->next;
	}
}

void llist_destroy(LLIST *handler)
{
	LLIST *cur = handler->next;
	while (cur != handler)
	{
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
		free(cur);
		cur = handler->next;
	}

	free(cur);
}

int llist_is_empty(LLIST *handler)
{
	return handler->next == handler;
}

int llist_de(LLIST *handler, void *save)
{
	LLIST *cur = handler->next;

	if (llist_is_empty(handler)) return -1;

	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;

	memcpy(save, &cur->id, sizeof(cur->id));

	free(cur);
	return 0;
}