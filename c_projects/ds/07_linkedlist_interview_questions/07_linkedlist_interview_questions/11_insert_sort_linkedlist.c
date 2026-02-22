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
    if (handler == NULL || handler->next == NULL)
        return;

    struct llist_node *cur = handler->next;  // cur 指向每一个数据节点
    struct llist_node *cur_next = cur->next; // cur_next 指向 cur 的下一个数据节点
    struct llist_node *ordered = handler;  // ordered 指向有序链表的头节点
   
    handler->next = NULL;  // 断开头节点与后面数据节点的连接, 构建有序链表

    while (cur != NULL)
    {
        cur_next = cur->next;  // 保存下一个要处理的节点
        ordered = handler;  // 每次都从头开始查找

        // 找插入位置
        while (ordered->next != NULL && ordered->next->val < cur->val)
            ordered = ordered->next;

        // 插入当前节点
        cur->next = ordered->next;
        ordered->next = cur;

        // 移动到下一个待处理节点
        cur = cur_next;
    }
}