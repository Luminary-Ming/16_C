#include "llist.h"
#include <stdio.h>
#include <string.h>

#define HEADINSERT 0  // 头插
#define TAILINSERT 1  // 尾插
#define NAMESIZE 20

typedef struct stu  // 数据节点结构体
{
	int id;  // 数据域
	char name[NAMESIZE];  // 数据域
	int math;  // 数据域
	struct list_head node;  // 指针域
}STU;

struct list_head *llist_create(void);
int llist_insert(struct list_head *, const void *, int);
void llist_display(struct list_head *);
void llist_destroy(struct list_head *);
STU *llist_find(struct list_head *, const void *);
int llits_delete(struct list_head *, const void *);
int llist_fetch(struct list_head *, const void *, void *);

int main(void)
{
	// 1. 创建头节点
	struct list_head *handler = llist_create();  

	STU *data = NULL;  // data 指向每一个数据节点
	for (int i = 0; i < 5; i++)
	{
		data = malloc(sizeof(STU));  // 开辟空间
		data->id = 100 + i;  // 录入数据
		sprintf(data->name, "stu%d",i);
		data->math = 100 - i;
		llist_insert(handler, data, TAILINSERT);  // 插入数据节点
	}

	// 2. 遍历链表
	llist_display(handler);

	printf("=========================================\n");


	// 3. 查找节点
	int find_id = 102;
	char *find_name = "stu4";
#if 0
	STU *find;
	find = llist_find(handler, find_name);  // 查找节点
	if (find == NULL)
		printf("Not Find It!\n");
	else
		printf("\nFind It! %d  %s  %d\n", find->id, find->name, find->math);

#elif 1
	// 4. 删除节点
	int ret = llits_delete(handler, &find_id);
	if (ret != 0)
		printf("Delete It Is Failed!\n");
	else
		printf("Delete It!\n");


#elif 0

	// 5. 取出节点
	STU save;  // 存储取出的数据节点
	int ret = llist_fetch(handler, &find_id, &save);
	if (ret != 0)
		printf("Fetch It Is Failed!\n");
	else
		printf("Fetch It!\n%d  %s  %d\n", save.id, save.name, save.math);
#endif

	printf("\n=========================================\n");


	// 6. 销毁节点
	llist_destroy(handler);
}

struct list_head *llist_create(void)
{
	struct list_head *handler = malloc(sizeof(struct list_head));
	if (handler == NULL)
		return NULL;  // 开辟失败
	INIT_LIST_HEAD(handler);  // 调用内核链表方法
	return handler;
}

int llist_insert(struct list_head *handler, const void *data, int mode)
{
	switch (mode)
	{
		case HEADINSERT:
			list_add(&((struct stu *)data)->node, handler);
			break;
		case TAILINSERT:
			list_add_tail(&((struct stu *)data)->node, handler);
			break;
		default: return -1;
	}
	return 0;
}

void llist_display(struct list_head *handler)
{
	struct list_head *pos = NULL;  // 指向每一个数据节点的指针域
	STU *datap = NULL;  // 指向每一个数据节点的首地址(也是数据域的首地址)

	list_for_each(pos, handler)
	{
		/*
			已知 STU 结构体成员中的 node 的首地址, 也就是指针域 pos 指针的地址
			list_entry  求的就是已知结构体中某个成员的地址, 通过该地址求结构体的起始地址
			ptr         结构体中某个成员的地址         (pos)
			type        使用者的结构体类型             (STU)
			member      使用者的结构体中某个成员的名字 (node)
		*/
		datap = list_entry(pos, STU, node);  // 求 STU 结构体的首地址, 也就是数据节点的首地址
		printf("%d %s %d\n", datap->id, datap->name, datap->math);
	}
}

void llist_destroy(struct list_head *handler) {
	struct list_head *pos = NULL;  // 指向每一个数据节点的指针域
	STU *datap = NULL;  // 指向每一个数据节点的首地址(也是数据域的首地址)
	int count = 0;

	list_for_each(pos, handler)
	{
		datap = list_entry(pos, STU, node);
		printf("正在销毁节点 %d: ID=%d, Name=%s\n", ++count, datap->id, datap->name);
		__list_del(pos->prev, pos->next);  // 摘除当前数据节点
		free(datap);
	}
	printf("共销毁了 %d 个数据节点\n", count);
	free(handler);  // 释放头节点
	printf("头节点已释放，链表销毁完成！\n");
}


STU *llist_find(struct list_head *handler, const void *find_data)
{
	STU *datap = NULL;
	struct list_head *pos = NULL;
	list_for_each(pos, handler)
	{
		datap = list_entry(pos, STU, node);
		if (datap->id == *(int *)(find_data) || !strcmp(datap->name, find_data))
			return datap;
	}
	return NULL;
}

int llits_delete(struct list_head *handler, const void *find_data)
{
	STU *datap = NULL;
	datap = llist_find(handler, find_data);
	if (datap == NULL)
		return -1;

	list_del(&datap->node);
	printf("正在删除节点: ID=%d, Name=%s\n", datap->id, datap->name);

	free(datap);

	return 0;
}

int llist_fetch(struct list_head *handler, const void *find_data, void *save)
{
	STU *datap = NULL;
	datap = llist_find(handler, find_data);
	if (datap == NULL)
		return -1;

	__list_del(datap->node.prev, datap->node.next);

	memcpy(save, datap, sizeof(STU));

	free(datap);
	
	return 0;
}

