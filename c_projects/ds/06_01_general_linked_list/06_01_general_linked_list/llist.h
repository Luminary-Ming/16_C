#ifndef __LLIST_H
#define __LLIST_H

#define HEADINSERT 0
#define TAILINSERT 1

typedef void (*llist_op)(const void *);  // 函数指针
typedef int (*llist_cmp)(const void *, const void *);  // 函数指针

struct llist_node  // 数据节点结构
{
	void *data;  // 指向数据域
	struct llist_node *prev;  // 前驱指针
	struct llist_node *next;  // 后继指针
};

typedef struct llist_head  // 头节点结构
{
	int size;  // 客户储存数据的指定大小
	struct llist_node head;  // 头节点(不是指针)
}LLIST;

LLIST *llist_create(int);  // 在 C 中, 声明函数可以不写参数名, 只写参数类型

int llist_insert(LLIST *, const void *, int);

void llist_display(LLIST *, llist_op);

void llist_destroy(LLIST *);

void *llist_find(LLIST *, const void *, llist_cmp);

int llist_delete(LLIST *, const void *, llist_cmp);

int llist_fetch(LLIST *, const void *, llist_cmp, void *);

#endif