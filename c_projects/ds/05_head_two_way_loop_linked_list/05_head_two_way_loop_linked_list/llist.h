#ifndef __LLIST_H
#define __LLIST_H

#define HEADINSERT 0  // 头插
#define TAILINSERT 1  // 尾插
#define NAMESIZE 20

typedef struct llist_node
{
	int id;
	char name[NAMESIZE];
	int math;
	struct llist_node *prev;  // 前驱指针
	struct llist_node *next;  // 后继指针
}LLIST;

LLIST *llist_create(void);

int llist_insert(LLIST *handler, const void *data, int mode);

void llist_display(LLIST *handler);

void llist_destroy(LLIST *handler);

LLIST *llist_find(LLIST *handler, const void *find_data);

int llist_delete(LLIST *handler, const void *find_data);

int llist_fetch(LLIST *handler, const void *find_data, void *save);

#endif