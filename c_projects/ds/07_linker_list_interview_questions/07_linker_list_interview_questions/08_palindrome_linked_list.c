/*
    8. 现有一个 无头单向不循环链表, 首节点是 l,
    如果链表是一个回文结构就返回真值, 如果链表不是回文结构就返回假值
    (如果链表只存在一个节点也算回文结构)
*/
#include <stdlib.h>

struct llist_node
{
    int val;
    struct llist_node *next;
};

struct llist_node *func(struct llist_node *l)
{
	struct llist_node *fast = l;  // 快指针
	struct llist_node *slow = l;  // 慢指针

    if (l->next == NULL || l == NULL)  // 如果链表只有一个首节点或者没有节点, 返回真
        return 1;
     
    while (fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;  // 快指针走两步
        slow = slow->next;  // 慢指针走一步
    }

    fast = slow->next;  // 让 fast 指向 slow 的下一个节点
    slow->next = NULL;  // 断开 slow 的 next 指向 NULL

    struct llist_node *fast_next = NULL;  // 指向 fast 的下一个节点
    while (fast != NULL)  // 反转链表中间节点以后的节点 
    {
        fast_next = fast->next;  // fast_next 指向 fast 的下一个节点
        fast->next = slow;  // fast 的 next 断开指向 slow
        slow = fast;  // slow 移动到 fast 的位置
        fast = fast_next;  // fast 移动到 fast_next 的位置
    }

    fast = l;  // fast 移动到首节点的位置

    while (fast != NULL && slow != NULL)  // 两指针从两头往中间走, 比较节点的数据是否相同
    {
        if (fast->val != slow->val)
            return 0;  // 发现不相等的节点, 返回 0 不是回文结构
        fast = fast->next;
        slow = slow->next;
    }

	return 1;  // 是回文结构
}