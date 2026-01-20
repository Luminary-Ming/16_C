#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LLIST *llist_create(void)
{
	LLIST *handler = NULL;  // handler 指针指向开辟的头节点
	handler = malloc(sizeof(LLIST));  // 开辟头节点
	if (handler == NULL) return NULL;  // 开辟失败

	handler->prev = handler->next = handler;  // 让前驱指针和后继指针都指向自己
	return handler;
}

int llist_insert(LLIST *handler, const void *data, int mode)
{
	LLIST *newnode = NULL;  // newnode 指针指向新开辟的节点
	LLIST *p = handler;  // p 指针代替 handler 指针做操作

	newnode = malloc(sizeof(LLIST));  // 开辟新节点
	if (newnode == NULL) return -1;  // 开辟新节点失败, 返回-1

	memcpy(newnode, data, sizeof(LLIST));  // 拷贝数据

	switch (mode)
	{
		case HEADINSERT: break;
		case TAILINSERT: p = p->prev; break;  // 头节点的前一个就是最后一个节点
		default:free(newnode); return -2;  // 选择插入模式失败, 释放开辟新节点的空间, 返回-2
	}
	newnode->next = p ->next;
	newnode->prev = p;
	newnode->next->prev = newnode;
	newnode->prev->next = newnode;

	return 0;
}

void llist_display(LLIST *handler)
{
	LLIST *cur = NULL;
	for (cur = handler->next; cur != handler; cur = cur->next)
		printf("%d %s %d\n", cur->id, cur->name, cur->math);
}

void llist_destroy(LLIST *handler)
{
	LLIST *cur = NULL;  // cur 指针指向要释放的节点
	for (cur = handler->next; cur != handler; cur = handler->next)
	{
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
		free(cur);  // 释放数据节点
	}
	free(cur);  // 释放头节点
}

static LLIST *_find(LLIST *handler, const void *find_data)
{
	LLIST *cur = NULL;  // cur 指针指向要找的节点
	for (cur = handler->next; cur != handler; cur = cur->next)
		if (cur->id == *(int *)(find_data) || !strcmp(cur->name, find_data))
			return cur;
	return NULL;
}

LLIST *llist_find(LLIST *handler, const void *find_data)
{
	return _find(handler, find_data);
}

int llist_delete(LLIST *handler, const void *find_data)
{
	LLIST *cur = NULL;  // cur 指针指向要删除的节点
	cur = _find(handler, find_data);
	if (cur == NULL) return -1;  // 没找到要删除的节点, 返回-1

	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	free(cur);

	return 0;
}

int llist_fetch(LLIST *handler, const void *find_data, void *save)
{
	LLIST *cur = NULL;  // cur 指针指向要取出的节点
	
	cur = _find(handler, find_data);  // 查询要取出的节点
	if (cur == NULL) return -1;  // 没有找到要取出的节点, 返回-1

	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	memcpy(save, cur, sizeof(LLIST));
	free(cur);

	return 0;
}