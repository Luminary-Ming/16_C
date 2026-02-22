/*
	3. 链表的中间节点
	给你一个 有头单向不循环链表 的头节点 handler,
	返回链表的中间节点, 如果有两个返回第二个中间节点
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

	// fast != NULL (链表节点个数是偶数)
	// fast->next != NULL (链表节点个数是奇数)
	while (fast != NULL && fast->next != NULL)
	{
		fast = fast->next->next;  // 快指针走两步
		slow = slow->next;  // 慢指针走一步
	}
	return slow;  // 返回慢指针
}