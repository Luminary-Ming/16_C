/*
    B. 链表排序 - 插入排序
    插入排序将第一个元素作为已经排序好的部分,
    然后依次将后面的元素插入到已排序的数列中, 直到所有成员都插入为止
*/
#include <stdlib.h>

struct llist_node
{
    int val;
    struct llist_node *next;
};

void sort(struct llist_node *handler)
{
    // 如果是无头链表记得创建一个头节点, 有头链表直接用 handler
    if (handler->next == NULL || handler == NULL)
        return;

    struct llist_node *cur = handler->next;  // cur 指向第一个数据节点
    struct llist_node *next = cur->next;  // next 指向 cur 的下一个节点
    struct llist_node *order = NULL;  // order 指向有序的链表

    cur->next = NULL;  // 断开旧链表的链接
    cur = next;  // cur 移动到 next 的位置

    while (cur != NULL)
    {
        order = handler;  // order 在有序链表中先标记 handler 的位置
        next = cur->next;  // next 指针后移
        while (order->next != NULL && order->next->val < cur->val)
            order = order->next;
        cur->next = order->next;
        order->next = cur;
        cur = next;
    }
} 