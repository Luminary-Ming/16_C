/*
    7. 拆分链表
    现有一个 无头单向不循环链表, 它的首节点是 l, 给定一个 val 值,
    将小于 val 值的节点排在前面, 大于 val 值的节点排在后面,
    不能改变链表原来的顺序, 返回重新整理过的链表的首节点
*/
#include <stdlib.h>

struct llist_node
{
    int val;
    struct llist_node *next;
};

struct llist_node *func(struct llist_node *l, int val)
{
    struct llist_node *cur = l;  // 指向旧链表的每一个节点
    struct llist_node *min_head = NULL;  // min_head 指向小链表
    struct llist_node *max_head = NULL;  // max_head 指向大链表
    struct llist_node *min_cur = NULL;  // min_cur 指向小链表当前操作的节点
    struct llist_node *max_cur = NULL;  // max_cur 指向大链表当前操作的节点

    min_head = malloc(sizeof(struct llist_node));  // 创建小链表的头
    max_head = malloc(sizeof(struct llist_node));  // 创建大链表的头

    min_cur = min_head;  // min_cur 代替 min_head 做操作
    max_cur = max_head;  // max_cur 代替 max_head 做操作

    while (cur != NULL)  // 循环遍历旧链表的每个节点
    {
        if (cur->val < val)  // 判断当前节点是否小于 val
        {
            min_cur->next = cur;  // 旧链表上的节点挂到小链表上
            min_cur = min_cur->next;  // min_cur 向后移动
        }
        else
        {
            max_cur->next = cur;   // 旧链表上的节点挂到大链表上
            max_cur = max_cur->next;   // max_cur 向后移动
        }
        cur = cur->next;  // cur 向后移动
    }

    min_cur->next = max_head->next;  // 小链表最后一个数据节点挂到大链表头节点的下一个节点上
    max_cur->next = NULL;  // 让大链表的最后一个节点的 next 指向 NULL
    l = min_head->next;  // l 指向小链表的第一个数据节点(头节点的 next)

    free(min_head);  // 释放开辟的内存
    free(max_head);  // 释放开辟的内存

    return l;
}