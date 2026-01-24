#ifndef __LLIST_H
#define __LLIST_H

#define HEADINSERT 0
#define TAILINSERT 1

typedef void (*llist_op)(const void *);  // 函数指针
typedef int (*llist_cmp)(const void *, const void *);  // 函数指针
typedef void LLIST;  // 把结构体类型换成 void 类型, 同时把结构体藏进 llist.c 中, 将来封装成库文件

LLIST *llist_create(int);

int llist_insert(LLIST *, const void *, int);

void llist_display(LLIST *, llist_op);

void llist_destroy(LLIST *);

void *llist_find(LLIST *, const void *, llist_cmp);

int llist_delete(LLIST *, const void *, llist_cmp);

int llist_fetch(LLIST *, const void *, llist_cmp, void *);

#endif