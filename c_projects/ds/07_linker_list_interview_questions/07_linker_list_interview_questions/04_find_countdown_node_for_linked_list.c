/*
	4. 输出倒数第 k 个节点
	给你一个 有头单向不循环链表 的头节点 handler,
	输出链表的倒数第 k 个节点
*/
#include <stdlib.h>

struct llist_node
{
	int val;
	struct llist_node *next;
};

struct llist_node *func(struct llist_node *handler, int k)
{
	struct llist_node *fast = handler;  // 快指针
	struct llist_node *slow = handler;  // 慢指针

	while (k--)  // 先让快指针走 k 步
	{
		if (fast == NULL)  // 链表节点个数少于 k 个
			return NULL;
		fast = fast->next;
	}

	while (fast != NULL)  // 再让快慢指针一起走
	{
		fast = fast->next;
		slow = slow->next;
	}

	return slow;  // 返回慢指针
}
/*
	假设链表长度为 n，我们要找倒数第 k 个节点，即正数第 n-k+1 个节点（从1开始计数）
	n 1 2 3 4 5 6 7
	k         3
	n-k+1 = 7-3+1 = 5
*/