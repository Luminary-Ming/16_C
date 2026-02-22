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

int func(struct llist_node *l)
{
    if (l == NULL || l->next == NULL)  // 如果链表只有一个首节点或者没有节点(空链表), 返回真
        return 1;
    
    // 找到链表的中间节点
    struct llist_node *fast = l;  // 快指针
    struct llist_node *slow = l;  // 慢指针
    while (fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;  // 快指针走两步
        slow = slow->next;  // 慢指针走一步
    }

    // 反转中间后半部分链表
    struct llist_node *cur = slow;  // cur 指向中间节点
    struct llist_node *cur_prev = NULL;  // cur_prev 指向 cur 的前一个节点
    struct llist_node *cur_next = NULL;  // cur_next 指向 cur 的下一个节点
    while (cur != NULL)
    {
        cur_next = cur->next;  // cur_next 指向 cur 的下一个节点
        cur->next = cur_prev;  // cur 指向 cur_prev
        cur_prev = cur;  // cur_prev 移动到 cur
        cur = cur_next;  // cur 移动到 cur_next
    }

    // 比较前半部分和反转后的后半部分
    struct llist_node *left = l;
    struct llist_node *right = cur_prev;
    while (right != NULL)  // 偶数节点 right 会先到 NULL
    {
        if (left->val != right->val)
            return 0;  // 发现有不相同的 val 说明不是回文结构
        left = left->next;
        right = right->next;
    }

    return 1;  // 是回文结构
}