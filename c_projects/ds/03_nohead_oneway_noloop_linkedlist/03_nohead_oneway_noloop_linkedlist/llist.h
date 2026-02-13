#ifndef LLIST_H
#define LLIST_H

#define NAMESIZE 20

#define HEADINSERT 0
#define TAILINSERT 1

enum llist_error
{
	LLIST_OK = 0,
	LLIST_ERR_MALLOC = -1,
	LLIST_ERR_PARAM = -2,
	LLIST_ERR_MODE = -3,
	LLIST_ERR_NOT_FOUND = -4
};

typedef struct llist_node
{
	// 数据域
	int id;
	char name[NAMESIZE];
	int math;
	// 指针域(指向下一个节点)
	struct llist_node *next;
}LLIST;  // struct llist_node 结构体类型的别名

int llist_insert(LLIST **, const void *, int);  // 插入

void llist_display(LLIST *);  // 遍历

LLIST *llist_find(LLIST *, const void *);  // 查找

int llist_fetch(LLIST **, const void *, void *);  // 取出

int llist_delete(LLIST **, const void *);  // 删除

void llist_destroy(LLIST *);  // 销毁

#endif