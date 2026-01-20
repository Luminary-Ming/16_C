#include "llist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

LLIST *llist_insert(LLIST *l, const void *data, int mode)
{
	if (l == NULL)  // 当前链表是空链表
	{
		l = malloc(sizeof(LLIST));  // 开辟第一个数据节点
		if (l == NULL) return NULL;  // 创建失败, 返回 NULL
		memcpy(l, data, sizeof(LLIST));  // 拷贝数据
		return l;  // 返回开辟成功的第一个数据节点
	}


	// 不是空链表
	LLIST *newnode = NULL;  // newnode 指针指向新开辟的节点
	LLIST *p = l;  // p 指针代替 l 指针做操作

	newnode = malloc(sizeof(LLIST));  // 开辟数据节点
	if (newnode == NULL)
		return l;
	memcpy(newnode, data, sizeof(LLIST));
	switch (mode)
	{
		case HEADINSERT: newnode->next = l; l = newnode; break;
		case TAILINSERT: while (p->next != NULL) p = p->next; p->next = newnode; break;
		default: free(newnode); return l;  // 选择插入模式失败, 释放新开辟的节点, 返回链表的第一个数据节点
	}
	return l;
}

void llist_display(LLIST *l)
{
	LLIST *cur = NULL;  // cur 指针指向每一个数据节点
	for (cur = l; cur != NULL; cur = cur->next)
		printf("%d %s %d\n", cur->id, cur->name, cur->math);
}

void llist_destroy(LLIST *l)
{
	LLIST *cur = l;  // 指向要销毁的节点
	LLIST *next = l->next;  // 指向要销毁的节点的下一个位置

	while (next != NULL)
	{
		free(cur);
		cur = next;
		next = next->next;
	}
	free(cur);
}
 
static LLIST *_find(LLIST *l, const void *find_data)
{
	LLIST *cur = NULL;
	for (cur = l; cur != NULL; cur = cur->next)
		if (cur->id == *(int *)find_data || !strcmp(cur->name, find_data))
			return cur;
	return NULL;
}

LLIST *llist_find(LLIST *l, const void *find_data)
{
	return _find(l, find_data);
}

int llist_delete(LLIST **l, const void *find_data)
{
	LLIST *back = *l;  // 前指针 (始终指向 cur 前一个的节点)
	LLIST *cur = NULL;  // 指向要删除的节点

	cur = _find(*l, find_data);
	if (cur == NULL)
		return -1;

	if (cur == *l)  // 是否是第一个数据节点
	{
		*l = (*l)->next;  // 指向第二个数据节点 (使用二级指针就是为了修改这里 l 指针的指向, 不使用二级指针的话 l = l->next 修改的是局部变量)
		free(cur);  // 释放第一个数据节点
	}
	else
	{
		while (back->next != cur)
			back = back->next;
		back->next = cur->next;
		free(cur);
	}
	return 0;
}

int llist_fetch(LLIST **l, const void *find_data, void *save)
{
	LLIST *back = *l;  // 前指针
	LLIST *cur = NULL;  // 指向要取出的节点
	cur = _find(*l, find_data);
	if (cur == NULL) return -1;

	if (cur == *l)  // 是第一个数据节点
		*l = (*l)->next;  // 指向第二个数据节点
	else 
	{
		while (back->next != cur)
			back = back->next;
		back->next = cur->next;
	}
	memcpy(save, cur, sizeof(LLIST));  // 拷贝数据
	free(cur);  // 释放第一个数据节点
	return 0;
}