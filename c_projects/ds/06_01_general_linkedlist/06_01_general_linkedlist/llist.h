#ifndef LLIST_H
#define LLIST_H

#define HEADINSERT 0
#define TAILINSERT 1

typedef void(*llist_op)(const void *);
typedef int(*llist_cmp)(const void *, const void *);

enum llist_error
{
	LLIST_OK = 0,
	LLIST_ERR_MALLOC = -1,
	LLIST_ERR_PARAM = -2,
	LLIST_ERR_MODE = -3,
	LLIST_ERR_NOT_FOUND = -4
};

struct llist_node  // 数据节点结构
{
	void *data;  // 指向数据域
	struct llist_node *prev;  // 前驱指针
	struct llist_node *next;  // 后继指针
};

typedef struct llist_head  // 头节点结构
{
	int size;  //  储存客户结构体数据的大小
	struct llist_node head;  // 头节点
}LLIST;

LLIST *llist_create(int);  // 创建

int llist_insert(LLIST *, const void *, int);  // 插入

void llist_display(LLIST *, llist_op);  // 遍历

void *llist_find(LLIST *, const void *, llist_cmp);  // 查找

int llist_fetch(LLIST *, const void *, llist_cmp, void *);  // 取出

int llist_delete(LLIST *, const void *, llist_cmp);  // 删除

void llist_destroy(LLIST *);  // 销毁

#endif