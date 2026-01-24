/*
    A. 找到环的入口点
    给你一个单向链表的首节点 handler 判断链表中是否存在环状结构,
    如果是环状结构, 找到环的入口点
    (循环链表也属于环状结构,属于特殊的环状结构,首尾相连)
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
    struct llist_node *p = handler;  // 指针 p 从头节点开始移动

    while (fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;  // 快指针走两步
        slow = slow->next;  // 慢指针走一步

        if (fast == slow)  // 如果相遇了说明是环形
        {
            while (p != slow)  // 一直走到 p 和 slow 相遇, 相遇的地方就是入口点
            {
                p = p->next;
                slow = slow->next;
            }
            return p;
        }
    }
    return NULL;
}