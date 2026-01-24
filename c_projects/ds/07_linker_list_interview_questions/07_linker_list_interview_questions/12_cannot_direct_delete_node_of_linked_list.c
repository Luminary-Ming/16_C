/*
    C. 不能直接删除的单链表节点
    假设有一条 单向不循环链表, 只给了你一个指针 p 指向要删除的节点,
    问怎么把这个节点删除掉 ?
*/
#include <stdlib.h>
#include <string.h>

struct llist_node
{
    int val;
    struct llist_node *next;
};

void delete_node(struct llist_node *p)
{
    if (p == NULL || p->next == NULL)
        return;  // 如果 p 指向 NULL 或者 P 的下一个节点是 NULL, 无解, 直接返回

    struct llist_node *p_next = p->next;  // p_next 指针指向 p 的下一个节点

    memcpy(p, p_next, sizeof(struct llist_node));  // p 拷贝 p_next 节点的数据
    p->next = p_next->next;  // 摘除 p_next 节点
    free(p_next);  // 释放 p_next 节点
}