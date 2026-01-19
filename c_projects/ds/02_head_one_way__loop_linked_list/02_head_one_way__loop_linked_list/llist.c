#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int llist_create(LLIST **handler)
{
	*handler = malloc(sizeof(LLIST));  // 开辟头节点的空间
	if (*handler == NULL)
		return -1;
	(*handler)->next = *handler;  // 首尾相连

	return 0;
}

int llist_insert(LLIST *handler, const void *data, int mode) 
{
	LLIST *p = handler;  // 使用指针 p 代替 handler 指针做操作 (保证 handler 首地址不变)
	LLIST *newnode = NULL;  // newnode 指针指向 NULL
	newnode = malloc(sizeof(LLIST));  // newnode 指针指向新开辟的数据节点
	if (newnode == NULL) return -1;  // 新开辟的节点开辟失败, 结束函数, 返回-1

	memcpy(newnode, data, sizeof(LLIST));  // 把数据拷贝到新开辟的节点中

	switch (mode)
	{
		case HEADINSERT: break;
		case TAILINSERT: while (p->next != handler) p = p->next; break;
		default:free(newnode); return -2;  // 插入模式选择失败, 释放开辟的空间, 结束函数, 返回-2
	}
	newnode->next = p->next;
	p->next = newnode;
	return 0;
}

void llist_display(LLIST *handler) 
{
	LLIST *cur = NULL;  // cur 指针指向每一个数据节点
	for (cur = handler->next; cur != handler; cur = cur->next)
	{
		printf("%d %s %d\n", cur->id, cur->name, cur->math);
	}
}

void llist_destroy(LLIST *handler)
{
	LLIST *back = handler;  // 前指针
	LLIST *cur = handler->next;  // 当前要释放的节点

	while (cur != handler)  // 循环释放所有数据节点
	{
		back->next = cur->next;  // 把当前数据节点从链表中摘除 
		free(cur);  // 释放当前数据节点
		cur = back->next;  // 重新指向下一个数据节点
	}
	free(back);  // 释放头节点
}

static LLIST *_find(LLIST *handler, const void *find_data)
{
	LLIST *cur = NULL;  // cur 指针指向每个数据节点

	for (cur = handler->next; cur != NULL; cur = cur->next)
		if (cur->id == *(int *)find_data || !strcmp(cur->name, find_data))
			return cur;
	return NULL;
}

LLIST *llist_find(LLIST *handler, const void *find_data)
{
	return _find(handler, find_data);
}

int llist_delete(LLIST *handler, const void *find_data)
{
	LLIST *back = handler;  // 前指针
	LLIST *cur = NULL;  // cur 指针指向要删除的节点

	cur = _find(handler, find_data);  // 查找要删除的节点
	if (cur == NULL)
		return -1;  // 没找到, 返回 -1
	
	while (back->next != cur)  // 指针 back 到要删除节点的前一个位置
		back = back->next;

	back->next = cur->next;  // 把要删除的节点从链表中摘除
	free(cur);

	return 0;
}

int llist_fetch(LLIST *handler, const void *find_data, void *save)
{
	LLIST *back = handler;  // 前指针
	LLIST *cur = NULL;  // cur 指针指向要取出数据的节点

	cur = _find(handler, find_data);  // 查找要取出数据的节点
	if (cur == NULL)
		return -1;  // 没找到, 返回 -1

	while (back->next != cur)  // 指针 back 到要取出节点的前一个位置
		back = back->next;

	back->next = cur->next;  // 把要取出的节点从链表中摘除
	memcpy(save, cur, sizeof(LLIST));  // 把要取出数据节点的数据拷贝出来
	free(cur);

	return 0;
}