#ifndef __CONTACT_LLIST_H
#define __CONTACT_LLIST_H

#define HEADINSERT 0
#define TAILINSERT 1

#include "contact.h"

typedef void (*llist_contact_print)(const void *);  // 函数指针

struct llist_node  // 数据节点结构体
{
	struct llist_node *prev;
	struct llist_node *next;
	char contact[0];  // 标记联系人数据首地址
};

typedef struct llist_head  // 头节点结构体
{
	int size;  // 联系人数据大小
	struct llist_node head;  // 头节点
	int count;  // 记录节点数
}LLIST;

LLIST *llist_create(int);  // 创建链表
int llist_insert(LLIST *, Contact *, int );  // 新增节点
void llist_display(LLIST *,llist_contact_print);  // 遍历链表
void llist_destroy(LLIST *);  // 销毁链表
void llist_clear(LLIST *);  // 清空链表
int llist_delete(LLIST *, int);  // 根据 id 删除节点
Contact *llist_find(LLIST *, int);  // 根据 id 查找节点
int llist_get_count(LLIST *);  // 获取节点数量
Contact *llist_get_at(LLIST *, int);  // 根据下标获取节点所在的位置
 
#endif 