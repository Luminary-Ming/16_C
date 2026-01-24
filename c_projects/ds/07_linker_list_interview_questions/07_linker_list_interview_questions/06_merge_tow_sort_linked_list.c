/*
	6. 合并两个有序链表
	将两个 升序 的 无头单向不循环链表, 合并为一个新的升序链表并返回
*/
#include <stdlib.h>

struct llist_node
{
	int val;
	struct llist_node *next;
};

struct llist_node *func(struct llist_node *l1, struct llist_node *l2)
{
	struct llist_node *l = NULL;  // l 指向新链表的首节点
	struct llist_node *cur = NULL;  // cur 指向正在操作的节点

	if (l1 == NULL)
		return l2;  // l1 不存在返回 l2
	if (l2 == NULL)
		return l2;  // l2 不存在返回 l1

	if (l1->val < l2->val)  // 判断谁小谁当新链表的首节点
	{
		l = cur = l1;  // l1 的首节点做 l 的首节点
		l1 = l1->next;  // l1 首节点后移
	}
	else
	{
		l = cur = l2;  // l2 的首节点做 l 的首节点
		l2 = l2->next;  // l2 首节点后移
	}

	while (l1 != NULL && l2 != NULL)   // 循环拆 l1 l2 链表的节点到新链表中
	{
		if (l1->val < l2->val)
		{
			cur->next = l1;
			l1 = l1->next;  // l1 后移
		}
		else
		{
			cur->next = l2;
			l2 = l2->next;  // l2 后移
		}
		cur = cur->next;  // cur 指向 l 链表的下一个节点
	}

	if (l1 == NULL)  // 如果 l1 链表先结束
		cur->next = l2;  // 把 l2 链表的剩余节点接到 l 链表后面
	else
		cur->next = l1;  // 把 l1 链表的剩余节点接到 l 链表后面

	return l;  // 返回新链表首节点 l
}