/*
	无头单向不循环链表
	无头 : 没有头节点
	单向 : 链表中每一个节点都只有一个指向下一个节点的指针
	不循环 : 链表中的最后一个节点指向 NULL
*/
#include <stdio.h>
#include "llist.h"

// 调试用
#define EXECUTE 2

int main(void)
{
	LLIST *l = NULL;  // l 指针指向无头链表的第一个数据节点

	LLIST data;  // data 变量存储录入的数据
	for (int i = 0; i < 5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name, "stu%d", i);
		data.math = 100 - i;
		data.next = NULL;  // 初始指针指向 NULL
		llist_insert(&l, &data, TAILINSERT);  // 插入数据节点
	}

	llist_display(l);  // 遍历链表

	printf("----------------------------------\n");

	int find_id = 100;  // 模拟要查找的 ID 号
	char *find_name = "stu4";  // 模拟要查找的名字

#if EXECUTE == 0
	LLIST *find = llist_find(l, find_name);
	if (!find)
		printf("Not found: name = %s\n", find_name);
	else
		printf("Found: id = %d, name = %s, math = %d\n", find->id, find->name, find->math);

#elif EXECUTE == 1
	LLIST save;  // save 变量储存取出节点的数据
	int ret = llist_fetch(&l, &find_id, &save);
	if (ret != 0)
		printf("Fetch failed: id = %d\n", find_id);
	else
		printf("Fetch ok: id = %d, name = %s, math = %d\n", save.id, save.name, save.math);

#elif EXECUTE == 2
	int ret = llist_delete(&l, &find_id);
	if (ret != 0)
		printf("Delete failed: id = %d\n", find_id);
	else
		printf("Delete ok: id = %d\n", find_id);

#endif
	printf("----------------------------------\n");

	llist_display(l);  // 遍历链表

	llist_destroy(l);  // 销毁链表

	return 0;
}