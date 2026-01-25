#ifndef __LLIST_H
#define __LLIST_H

#define HEADINSERT 0  // 头插
#define TAILINSERT 1  // 尾插
#define NAMESIZE 20

typedef struct llist_node
{
	int id;
	struct llist_node *prev;  // 前驱指针
	struct llist_node *next;  // 后继指针
}LLIST;

LLIST *llist_create(void);

int llist_insert(LLIST *handler, const void *data, int mode);

void llist_display(LLIST *handler);

void llist_destroy(LLIST *handler);

int llist_is_empty(LLIST *handler);

int llist_pop(LLIST *handler, void *save);

#endif