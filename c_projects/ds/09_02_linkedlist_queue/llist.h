#ifndef LLIST_H
#define LLIST_H

#define HEADINSERT 0
#define TAILINSERT 1 
#define NAMESIZE 20

typedef struct llist_node
{
	int id;
	struct llist_node *prev;
	struct llist_node *next;
}LLIST;

LLIST *llist_create(void);

int llist_insert(LLIST *handler, const void *data, int mode);

void llist_display(LLIST *handler);

void llist_destroy(LLIST *handler);

int llist_is_empty(LLIST *handler);

int llist_de(LLIST *handler, void *save);

#endif