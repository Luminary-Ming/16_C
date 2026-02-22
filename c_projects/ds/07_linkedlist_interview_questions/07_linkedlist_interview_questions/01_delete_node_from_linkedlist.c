/*
	1. 删除链表节点
	给你一个 无头单向不循环链表 的首节点 l 和一个数据 val
	把该链表中所有等于 val 的节点删掉, 返回新的首节点
*/
#include <stdlib.h>

struct llist_node
{
	int val;
	struct llist_node *next;
};

struct llist_node *func(struct llist_node *l, int val)
{
	struct llist_node *cur = l;  // cur 指向当前要删除的节点
	struct llist_node *cur_prev = NULL;  // cur_prev 指向当前要删除的节点的前一个节点
	while (cur != NULL)  // 遍历所有节点
	{
		if (cur->val == val)  // 判断是否是要删除的节点
		{
			if (cur_prev == NULL)  // 要删除的节点是首节点
			{
				l = l->next;  // 首节点后移
				free(cur);  // 释放首节点
				cur = l;  // cur 指向新的首节点
			}
			else
			{
				cur_prev->next = cur->next;  // 摘除要删除的节点
				free(cur);  // 释放当前节点
				cur = cur_prev->next;  // cur 指向下一个数据节点
			}
		}
		else  // 当前节点不需要删除
		{
			cur_prev = cur;  // 更新前驱指针
			cur = cur->next;  // 移动到下一个节点
		}
	}
	return l;  // 返回新的首节点
}