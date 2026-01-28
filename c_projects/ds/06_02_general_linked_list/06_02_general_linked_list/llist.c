#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LLIST *llist_create(int size)
{
	LLIST *handler = NULL;  // handler 指针指向头节点
	handler = malloc(sizeof(LLIST));  // 开辟头节点空间
	if (handler == NULL)
		return NULL;  // 开辟失败, 返回 NULL

	handler->size = size;  // 客户储存数据的指定大小存到头节点中
	handler->head.prev = &handler->head;  // 让头节点的 prev 指针指向头节点中的 head
	handler->head.next = &handler->head;  // 让头节点的 next 指针指向头节点中的 head

	return handler;  // 返回开辟成功的头节点首地址
}

int llist_insert(LLIST *handler, const void *data, int mode)
{
	struct llist_node *p = &handler->head;  // p 指针指向头节点中的 head 做操作
	struct llist_node *newnode = NULL;  // newnode 指针指向新开辟的数据节点
	newnode = malloc(sizeof(LLIST) + handler->size);  // 开辟 LLIST 结构体大小 + 客户数据大小的空间
	if (newnode == NULL)
		return -1;

	memcpy(newnode->data, data, handler->size);  // 拷贝客户数据

	switch (mode)
	{
		case HEADINSERT: break;
		case TAILINSERT: p = p->prev; break;
		default: free(newnode); return -3;  // 插入失败, 释放空间
	}

	newnode->next = p->next;
	newnode->prev = p;
	newnode->next->prev = newnode;
	newnode->prev->next = newnode;

	return 0;
}

void llist_display(LLIST *handler, llist_op op)
{
	struct llist_node *cur = NULL;  // cur 指针指向每一个数据节点

	for (cur = handler->head.next; cur != &handler->head; cur = cur->next)
		op(cur->data);  // 使用客户给的函数进行打印输出
}

void llist_destroy(LLIST *handler)
{
	struct llist_node *cur = handler->head.next;  // cur 指针指向每一个数据节点

	while (cur != &handler->head)
	{
		cur->next->prev = cur->prev;  // 摘除当前节点
		cur->prev->next = cur->next;
		free(cur);  // 释放数据节点
		cur = handler->head.next;  // 每次释放头节点的下一个节点
	}
	free(handler);  // 释放头节点
}

static struct llist_node *_find(LLIST *handler, const void *find_data, llist_cmp cmp)
{
	struct llist_node *cur = NULL;

	for (cur = handler->head.next; cur != &handler->head; cur = cur->next)
	{
		if (cmp(cur->data, find_data))
			return cur;  // 返回找到的数据节点地址
	}
	return NULL;
}

void *llist_find(LLIST *handler, const void *find_data, llist_cmp cmp)
{
	struct llist_node *cur = _find(handler, find_data, cmp);
	if (cur != NULL)
		return cur->data;
	else
		return NULL;
}

int llist_delete(LLIST *handler, const void *find_data, llist_cmp cmp)
{
	struct llist_node *cur = NULL;
	cur = _find(handler, find_data, cmp);  // 查找要删除的节点
	if (cur == NULL)
		return -1;

	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	free(cur);  // 释放数据节点

	return 0;
}

int llist_fetch(LLIST *handler, const void *find_data, llist_cmp cmp, void *save)
{
	struct llist_node *cur = NULL;
	cur = _find(handler, find_data, cmp);
	if (cur == NULL)
		return -1;

	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	memcpy(save, cur->data, handler->size);  // 拷贝数据
	free(cur);

	return 0;
}