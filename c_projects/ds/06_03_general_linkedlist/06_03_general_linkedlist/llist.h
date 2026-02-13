#ifndef LLIST_H
#define LLIST_H

#define HEADINSERT 0
#define TAILINSERT 1

typedef void(*llist_op)(const void *);
typedef int(*llist_cmp)(const void *, const void *);

// 把结构体类型换成 void 类型, 同时把结构体藏进 llist.c 中, 将来封装成库文件

void *llist_create(int);  // 创建

int llist_insert(void *, const void *, int);  // 插入

void llist_display(void *, llist_op);  // 遍历

void *llist_find(void *, const void *, llist_cmp);  // 查找

int llist_fetch(void *, const void *, llist_cmp, void *);  // 取出

int llist_delete(void *, const void *, llist_cmp);  // 删除

void llist_destroy(void *);  // 销毁

#endif