#ifndef LLIST_H
#define LLIST_H

#define NAMESIZE 20

// 定义宏, 表示头插法和尾插法
#define HEADINSERT 0
#define TAILINSERT 1

enum llist_error
{
	LLIST_OK = 0,  // 成功
	LLIST_ERR_MALLOC = -1,  // 开辟失败
	LLIST_ERR_PARAM = -2,  // 参数错误
	LLIST_ERR_MODE = -3,  // 插入模式错误
	LLIST_ERR_NOT_FOUND = -4  // 未找到
};

// 链表中每个数据节点的结构
typedef struct llist_node
{
	// 数据域
	int id;
	char name[NAMESIZE];
	int math;
	// 指针域(指向下一个节点)
	struct llist_node *next;
}LLIST;  // struct llist_node 结构体类型的别名

LLIST *llist_create(void);  // 创建

int llist_insert(LLIST *, const void *, int);  // 插入

void llist_display(LLIST *);  // 遍历

LLIST *llist_find(LLIST *, const void *);  // 查找

int llist_fetch(LLIST *, const void *, void *);  // 取出

int llist_delete(LLIST *, const void *);  // 删除

void llist_destroy(LLIST *);  // 销毁

#endif