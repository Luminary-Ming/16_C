#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LLIST_DEBUG

#ifdef LLIST_DEBUG
// 调试模式：可变参数宏, 用于打印错误日志
// __func__ 不是宏，是隐式变量标识符，C99 起支持
// __VA_ARGS__ 代表宏定义中 ... 对应的所有参数 (如果调用 LLIST_LOG("size = %d", size)，__VA_ARGS__ 就是 size)
// ## 在这里的特殊作用 : 如果可变参数为空，删除前面的逗号
#define LLIST_LOG(fmt, ...) \
	fprintf(stderr, "[ERROR] %s:%d - %s()" fmt "\n", __FILE__, __LINE__, __func__ , ##__VA_ARGS__)
#else
// 发布模式：空语句
// (void)0：把整数 0 强制转换成 void 类型, 整体是一个无返回值、无副作用、合法的C语句
// 编译后：不生成任何机器码，完全被优化掉
#define LLIST_LOG(fmt, ...) ((void)0)
#endif

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
	struct llist_node *prev;  // 前驱指针
	struct llist_node *next;  // 后继指针
	char data[0];  // 标记数据域的首地址, 柔性数组，不占空间
};

typedef struct llist_head  // 头节点结构
{
	int size;  //  储存客户结构体数据的大小
	struct llist_node head;  // 头节点
}LLIST;

void *llist_create(int size)
{
	LLIST *handler = malloc(sizeof(LLIST));
	if (!handler)
	{
		LLIST_LOG("malloc failed: %s", strerror(errno));
		return NULL;
	}
	handler->size = size;   // 储存客户结构体数据的大小
	handler->head.prev = &handler->head;  // 让头节点的 prev 指针指向头节点中的 head
	handler->head.next = &handler->head;  // 让头节点的 next 指针指向头节点中的 head

	return handler;
}

int llist_insert(void *h, const void *data, int mode)
{
	LLIST *handler = (LLIST *)h;  // 转换成结构体类型的指针
	struct llist_node *newnode = malloc(sizeof(struct llist_node) + handler->size);  // 开辟新的数据节点
	if (!newnode)
	{
		LLIST_LOG("malloc failed: %s", strerror(errno));
		return LLIST_ERR_MALLOC;
	}

	memcpy(newnode->data, data, handler->size);  // 拷贝客户数据

	struct llist_node *p = &handler->head;  // p 代替头节点做操作
	switch (mode)
	{
		case HEADINSERT: break;
		case TAILINSERT: p = p->prev; break;
		default:
			free(newnode);
			LLIST_LOG("mode failed: %s", strerror(errno));
			return LLIST_ERR_MODE;
	}
	newnode->prev = p;
	newnode->next = p->next;
	newnode->prev->next = newnode;
	newnode->next->prev = newnode;

	return LLIST_OK;
}

void llist_display(void *h, llist_op op)
{
	LLIST *handler = (LLIST *)h;  // 转换成结构体类型的指针
	struct llist_node *cur = handler->head.next;  // cur 指针指向每一个数据节点
	while (cur != &handler->head)
	{
		op(cur->data);  // 使用客户给的函数进行打印输出
		cur = cur->next;
	}
}

static struct llist_node *_find(LLIST *handler, const void *find_data, llist_cmp cmp)
{
	struct llist_node *cur = handler->head.next;  // cur 指针指向每一个数据节点
	while (cur != &handler->head)
	{
		if (cmp(cur->data, find_data))
			return cur;
		cur = cur->next;
	}
	return NULL;
}

void *llist_find(void *h, const void *find_data, llist_cmp cmp)
{
	LLIST *handler = (LLIST *)h;  // 转换成结构体类型的指针
	struct llist_node *cur = _find(handler, find_data, cmp);
	if (!cur)
		return NULL;
	return cur->data;
}

int llist_fetch(void *h, const void *find_data, llist_cmp cmp, void *save)
{
	LLIST *handler = (LLIST *)h;  // 转换成结构体类型的指针
	struct llist_node *cur = _find(handler, find_data, cmp);  // cur 指向要取出的节点
	if (!cur)
		return LLIST_ERR_NOT_FOUND;

	cur->prev->next = cur->next;  // 摘除节点
	cur->next->prev = cur->prev;

	memcpy(save, cur->data, handler->size);  // 拷贝数据

	free(cur);  // 释放数据节点

	return LLIST_OK;
}

int llist_delete(void *h, const void *find_data, llist_cmp cmp)
{
	LLIST *handler = (LLIST *)h;  // 转换成结构体类型的指针
	struct llist_node *cur = _find(handler, find_data, cmp);  // cur 指向要删除的节点
	if (!cur)
		return LLIST_ERR_NOT_FOUND;

	cur->prev->next = cur->next;  // 摘除节点
	cur->next->prev = cur->prev;

	free(cur);  // 释放数据节点

	return LLIST_OK;
}

void llist_destroy(void *h)
{
	LLIST *handler = (LLIST *)h;  // 转换成结构体类型的指针
	struct llist_node *cur = handler->head.next;  // cur 指针指向每一个数据节点
	while (cur != &handler->head)
	{
		cur->prev->next = cur->next;  // 摘除节点
		cur->next->prev = cur->prev;
		free(cur);  // 释放数据节点
		cur = handler->head.next;
	}
	free(handler);  // 最后释放头节点
}