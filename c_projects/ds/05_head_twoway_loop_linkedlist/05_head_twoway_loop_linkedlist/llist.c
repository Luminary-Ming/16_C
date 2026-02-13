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

LLIST *llist_create(void)
{
	LLIST *handler = malloc(sizeof(LLIST));
	if (!handler)
	{
		LLIST_LOG("malloc failed: %s", strerror(errno));
		return NULL;
	}

	handler->prev = handler->next = handler;  // 让前驱指针和后继指针都指向头节点
	return handler;
}

int llist_insert(LLIST *handler, const void *data, int mode)
{
	LLIST *newnode = malloc(sizeof(LLIST));
	if (!newnode)
	{
		LLIST_LOG("malloc failed: %s", strerror(errno));
		return LLIST_ERR_MALLOC;
	}

	memcpy(newnode, data, sizeof(LLIST));

	LLIST *p = handler;  // p 指针代替 handler 指针做操作
	switch (mode)
	{
		case HEADINSERT: break;
		case TAILINSERT: p = p->prev; break;  // 头节点的前一个就是最后一个节点
		default:
			free(newnode);
			LLIST_LOG("mode failed: %s", strerror(errno));
			return LLIST_ERR_MODE;
	}
	newnode->prev = p;
	newnode->next = p->next;
	newnode->prev->next = newnode;
	newnode->next->prev = newnode;

	return 0;
}

void llist_display(LLIST *handler)
{
	LLIST *cur = handler->next;  // 定义 cur 指针指向每一个数据节点
	while (cur != handler)
	{
		printf("id = %d, name = %s, math = %d\n", cur->id, cur->name, cur->math);
		cur = cur->next;
	}
}

// 内部静态方法
static LLIST *_find(LLIST *handler, const void *find_data)
{
	LLIST *cur = handler->next;  // 定义 cur 指针指向每一个数据节点
	while (cur != handler)
	{
		if (cur->id == *((int *)find_data) ||
			!strcmp(cur->name, (const char *)find_data) ||
			cur->math == *((int *)find_data))
			return cur;
		cur = cur->next;
	}
	return NULL;
}

LLIST *llist_find(LLIST *handler, const void *find_data)
{
	return _find(handler, find_data);
}

int llist_fetch(LLIST *handler, const void *find_data, void *save)
{
	LLIST *cur = _find(handler, find_data);  // 定义 cur 指针指向要取出的节点
	if (!cur)
		return LLIST_ERR_NOT_FOUND;

	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;

	memcpy(save, cur, sizeof(LLIST));  // 拷贝数据

	free(cur);  // 释放数据节点

	return LLIST_OK;
}

int llist_delete(LLIST *handler, const void *find_data)
{
	LLIST *cur = _find(handler, find_data);  // 定义 cur 指针指向要取出的节点
	if (!cur)
		return LLIST_ERR_NOT_FOUND;

	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	
	free(cur);  // 释放数据节点

	return LLIST_OK;
}

void llist_destroy(LLIST *handler)
{
	LLIST *cur = handler->next;  // 定义 cur 指针指向每一个数据节点
	while (cur != handler)
	{
		handler->next = cur->next;  // 摘除节点
		free(cur);
		cur = handler->next;  // cur 重新指向头节点的下一个节点
	}
	free(handler);  // 释放头节点
}