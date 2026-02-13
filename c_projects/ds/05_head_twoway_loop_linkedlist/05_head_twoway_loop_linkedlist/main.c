/*
	有头双向循环链表
	有头 : 有头节点
	双向 : 链表中每一个节点都有一个前驱指针和一个后继指针
	循环 : 链表中的最后一个节点指向头节点
*/

#include <stdio.h>
#include "llist.h"

// 调试用
#define EXECUTE 2

int main(void)
{
	LLIST *handler = llist_create();  // 创建头节点
	if (!handler)
	{
		printf("Create llist failed\n");
		return -1;
	}

	LLIST data;  // data 变量存储录入的数据
	for (int i = 0; i < 5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name, "stu%d", i);
		data.math = 100 - i;
		data.next = NULL;  // 初始指针指向 NULL
		llist_insert(handler, &data, TAILINSERT);  // 插入数据节点
	}

	llist_display(handler);  // 遍历链表

	printf("----------------------------------\n");

	int find_id = 100;  // 模拟要查找的 ID 号
	char *find_name = "stu4";  // 模拟要查找的名字

#if EXECUTE == 0
	LLIST *find = llist_find(handler, find_name);
	if (!find)
		printf("Not found: name = %s\n", find_name);
	else
		printf("Found: id = %d, name = %s, math = %d\n", find->id, find->name, find->math);

#elif EXECUTE == 1
	LLIST save;  // save 变量储存取出节点的数据
	int ret = llist_fetch(handler, &find_id, &save);
	if (ret != 0)
		printf("Fetch failed: id = %d\n", find_id);
	else
		printf("Fetch ok: id = %d, name = %s, math = %d\n", save.id, save.name, save.math);

#elif EXECUTE == 2
	int ret = llist_delete(handler, &find_id);
	if (ret != 0)
		printf("Delete failed: id = %d\n", find_id);
	else
		printf("Delete ok: id = %d\n", find_id);

#endif
	printf("----------------------------------\n");

	llist_display(handler);  // 遍历链表

	llist_destroy(handler);  // 销毁链表

	return 0;
}