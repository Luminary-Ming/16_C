/*
    9.环形链表
    给你一个单向链表的首节点是 handler 判断链表中是否存在环状结构
    (循环链表也属于环状结构, 属于特殊的环状结构, 首尾相连)
*/
#include <stdlib.h>

struct llist_node
{
    int val;
    struct llist_node *next;
};

struct llist_node *func(struct llist_node *handler)
{
	struct llist_node *fast = handler;  // 快指针
	struct llist_node *slow = handler;  // 慢指针

	while (fast != NULL && fast->next != NULL)
	{
		fast = fast->next->next;
		slow = slow->next;
		if (fast == slow)  // 如果相遇了说明是环形
			return 1;
	}

	return 0;
}