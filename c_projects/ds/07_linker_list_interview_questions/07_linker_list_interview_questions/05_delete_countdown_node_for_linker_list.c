/*
	5. 删除倒数第 k 个节点
	给你一个 有头单向不循环链表 的头节点 handler,
	删除链表的倒数第 k 个节点
*/
#include <stdlib.h>

struct llist_node
{
	int val;
	struct llist_node *next;
};

int func1(struct llist_node *handler, int k)
{
	struct llist_node *fast = handler;  // 快指针
	struct llist_node *slow = handler;  // 慢指针
	struct llist_node *slow_prev = NULL;  // slow_prev 指向慢指针的前一个节点

	while (k--)  // 先让快指针走 k 步
	{
		if (fast == NULL)  // 链表节点个数少于 k 个
			return -1;
		fast = fast->next;
	}

	while (fast != NULL)  // 再让快慢指针一起走
	{
		fast = fast->next;
		slow_prev = slow;
		slow = slow->next;
	}

	if (slow_prev == NULL)  // 删除的是头节点
	{
		free(slow);
	}
	else 
	{
		slow_prev->next = slow->next;  // 摘除节点
		free(slow);
	}

	return 0;
}

int func2(struct llist_node *handler, int k)
{
	struct llist_node *fast = handler;  // 快指针
	struct llist_node *slow = handler;  // 慢指针
	struct llist_node *delete = NULL;  // delete 指向要删除的节点

	  
	do  // 先让快指针走 k+1 步
	{
		if (fast == NULL)  // 链表节点个数少于 k+1 个
			return -1;
		fast = fast->next;
	} while (k--);

	while (fast != NULL)  // 再让快慢指针一起走
	{
		fast = fast->next;
		slow = slow->next;
	}

	delete = slow->next;  // delete 移动到要删除的节点上
	slow->next = delete->next;  // 摘除节点
	free(delete);  // 释放节点

	return 0;
}