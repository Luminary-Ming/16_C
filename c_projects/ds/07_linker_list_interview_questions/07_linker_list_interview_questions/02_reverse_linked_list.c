/*
	2. 反转链表
	给你一个 无头单向不循环链表 的首节点 l,
	反转这个链表, 返回新的首节点
*/
#include <stdlib.h>

struct llist_node
{
	int val;
	struct llist_node *next;
};

struct llist_node *func(struct llist_node *l)
{
	struct llist_node *cur = l;  // cur 指向要反转的节点
	struct llist_node *cur_prev = NULL;  // cur_prev 指向 cur 的前一个节点
	struct llist_node *cur_next = NULL;  // cur_next 指向 cur 的下一个节点

	while (cur != NULL)  // 遍历所有节点
	{
		cur_next = cur->next;  // cur_next 指向 cur 的下一个节点
		cur->next = cur_prev;  // cur 的 next 断开指向前一个节点
		cur_prev = cur;  // cur_prev 移动到 cur
		cur = cur_next;  // cur 移动到 cur_next
	}
	return cur_prev;  // 返回反转链表的首节点
}