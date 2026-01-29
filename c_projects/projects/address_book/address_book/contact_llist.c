#include "contact_llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LLIST *llist_create(int size)
{
    LLIST *handler = malloc(sizeof(LLIST));  // 开辟头节点的空间
    if (handler == NULL)
        return NULL;

    handler->size = size;
    handler->count = 0;
    handler->head.prev = &handler->head;
    handler->head.next = &handler->head;

    return handler;
}

int llist_insert(LLIST *handler, Contact *contact, int mode)
{
    if (!handler || !contact)
        return -1;

    struct llist_node *newnode = malloc(sizeof(struct llist_node) + handler->size);
    if (!newnode)
        return -1;

    contact->del = 0;  // 默认新增联系人是未删除状态
    memcpy(newnode->contact, contact, handler->size);

    struct llist_node *p = &handler->head;  // p 代替头节点做操作
    switch (mode)
    {
        case HEADINSERT: break;
        case TAILINSERT: p = p->prev; break;
        default: free(newnode); return -3;  // 插入失败, 释放空间
    }

    newnode->next = p->next;
    newnode->prev = p;
    newnode->next->prev = newnode;
    newnode->prev->next = newnode;

    handler->count++;  // 节点数量(下标)

    return 0;
}


void llist_display(LLIST *handler, llist_contact_print contact_print)
{
    if (!handler)
        return;

    printf("联系人数量: %d 个\n", handler->count);
    printf("====================\n");

    struct llist_node *cur = handler->head.next;
    while (cur != &handler->head)
    {
        Contact *contact = (Contact *)cur->contact;
        if (contact->del == 0)  // 只显示未删除的联系人
        {
            contact_print(contact);
        }
        cur = cur->next;
    }
}

Contact *llist_find(LLIST *handler, int id)
{
    if (!handler)
        return NULL;

    struct llist_node *cur = handler->head.next;
    while (cur != &handler->head)
    {
        Contact *contact = (Contact *)cur->contact;
        if (contact->id == id && contact->del == 0)  // 只返回未删除的联系人
        {
            return contact;
        }
        cur = cur->next;
    }
    return NULL;
}

int llist_get_count(LLIST *handler)
{
    if (!handler)
        return 0;
    return handler->count;
}

Contact *llist_get_at(LLIST *handler, int index)
{
    if (!handler || index < 0 || index >= handler->count)
        return NULL;

    struct llist_node *cur = handler->head.next;
    for (int i = 0; i < index; i++)
    {
        if (cur == &handler->head)
            return NULL;
        cur = cur->next;
    }
    return (Contact *)cur->contact;
}


void llist_destroy(LLIST *handler)
{
    if (!handler)
        return;

    struct llist_node *cur = handler->head.next;   // cur 指针指向每一个数据节点
    while (cur != &handler->head)
    {
        cur->next->prev = cur->prev;
        cur->prev->next = cur->next;
        free(cur);  // 释放数据节点
        cur = handler->head.next;
    }
    free(handler);  // 释放头节点
}

void llist_clear(LLIST *handler)
{
    if (!handler)
        return;
    struct llist_node *cur = handler->head.next;
    while (cur != &handler->head)
    {
        cur->next->prev = cur->prev;
        cur->prev->next = cur->next;
        free(cur);  // 释放数据节点
        cur = handler->head.next;
    }

    handler->head.prev = &handler->head;
    handler->head.next = &handler->head;
    handler->count = 0;
}

int llist_delete(LLIST *handler, int id)
{
    if (!handler)
        return -1;

    struct llist_node *cur = handler->head.next;
    while (cur != &handler->head)
    {
        Contact *contact = (Contact *)cur->contact;
        if (contact->id == id)
        {
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            free(cur);
            handler->count--;
            return 0;
        }
        cur = cur->next;
    }
    return -2;
}