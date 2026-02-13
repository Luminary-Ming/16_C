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

int llist_insert(LLIST **l, const void *data, int mode)
{
	// 当前链表是空链表
	if (*l == NULL)  
	{
		*l = malloc(sizeof(LLIST));  // 开辟第一个数据节点
		if (*l == NULL)
		{
			LLIST_LOG("malloc failed: %s", strerror(errno));
			return LLIST_ERR_MALLOC;  // 开辟失败, 返回 NULL
		}

		memcpy(*l, data, sizeof(LLIST));  // 拷贝数据

		return LLIST_OK;
	}

	// 不是空链表
	LLIST *newnode = malloc(sizeof(LLIST));  // newnode 指针指向新开辟的节点
	if (!newnode)
	{
		LLIST_LOG("malloc failed: %s", strerror(errno));
		return LLIST_ERR_MALLOC;
	}

	memcpy(newnode, data, sizeof(LLIST));  // 拷贝数据

	LLIST *p = *l;  // p 指针代替第一个数据节点做操作
	switch (mode)
	{
		case HEADINSERT:
			newnode->next = *l;
			*l = newnode;
			break;
		case TAILINSERT:
			while (p->next != NULL)
				p = p->next;
			p->next = newnode;
			newnode->next = NULL;
			break;
		default: 
			free(newnode);
			LLIST_LOG("mode failed: %s", strerror(errno));
			return LLIST_ERR_MODE;
	}
	return LLIST_OK;
}

void llist_display(LLIST *l)
{
	LLIST *cur = l;  // 定义 cur 指针指向每一个数据节点
	while (cur != NULL)
	{
		printf("id = %d, name = %s, math = %d\n", cur->id, cur->name, cur->math);
		cur = cur->next;
	}
}

// 内部静态方法
static LLIST *_find(LLIST *l, const void *find_data)
{
	LLIST *cur = l; // 定义 cur 指针指向每一个数据节点
	while (cur != NULL)
	{
		if (cur->id == *((int *)find_data) ||
			!strcmp(cur->name, (const char *)find_data) ||
			cur->math == *((int *)find_data))
			return cur;
		cur = cur->next;
	}
	return NULL;
}

LLIST *llist_find(LLIST *l, const void *find_data)
{
	return _find(l, find_data);
}

int llist_fetch(LLIST **l, const void *find_data, void *save)
{
	LLIST *cur = _find(*l, find_data);  // 定义 cur 指针指向要取出的节点
	if (!cur)
		return LLIST_ERR_NOT_FOUND;

	if (cur == *l)  // 如果要取出的节点是第一个数据节点
		*l = (*l)->next;  // l 后移
	else 
	{
		LLIST *back = *l;  // 定义 back 指针指向 l 的前一个节点
		
		while (back->next != cur)
			back = back->next;

		back->next = cur->next;  // 摘除要取出的数据节点
	}

	memcpy(save, cur, sizeof(LLIST));  // 拷贝数据

	free(cur);  // 释放数据节点

	return LLIST_OK;
}

int llist_delete(LLIST **l, const void *find_data)
{
	LLIST *cur = _find(*l, find_data);  // 定义 cur 指针指向要取出的节点
	if (!cur)
		return LLIST_ERR_NOT_FOUND;

	if (cur == *l)  // 如果要删除的节点是第一个数据节点
		*l = (*l)->next;  // l 后移
	else 
	{
		LLIST *back = *l;  // 定义 back 指针指向 l 的前一个节点

		while (back->next != cur)
			back = back->next;

		back->next = cur->next;  // 摘除要删除的数据节点
	}

	free(cur);  // 释放数据节点

	return LLIST_OK;
}

void llist_destroy(LLIST *l)
{
	while (l != NULL)
	{
		LLIST *next = l->next;  // next 指针指向 l 的下一个节点
		free(l);
		l = next;  // l 重新指向下一个节点
	}
}