/*
	无头单向不循环链表
*/
#include <stdio.h>
#include "llist.h"

int main(void)
{
	LLIST *l = NULL;  // l 指针指向无头单向不循环链表的第一个数据节点
	LLIST data;  // data 存储用户录入的数据
	LLIST save;  // save 储存取出的数据
	LLIST *find = NULL;  // find 指针指向要查找的节点
	int ret = 0;

	int find_id = 100;
	char *find_name = "stu4";

	for (int i = 0; i < 5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name, "stu%d", i);
		data.math = 100 - i;
		data.next = NULL;
		l = llist_insert(l, &data, TAILINSERT);
	}

	llist_display(l);

	printf("------------------------\n");
#if 0
	find = llist_find(l, find_name);
	if (find == NULL)
		printf("Not Find It!\n");
	else
		printf("Find It! %d  %s  %d\n", find->id, find->name, find->math);

#elif 0
	ret = llist_delete(&l, find_name);
	if (ret != 0)
		printf("Delete It Is Failed!\n");
	else
		printf("Delete It!\n");

#elif 1
	ret = llist_fetch(&l, find_name, &save);
	if (ret != 0)
		printf("Fetch It Is Failed!\n");
	else
		printf("Fetch It! %d  %s  %d\n", save.id, save.name, save.math);
#endif
	printf("------------------------\n");

	llist_display(l);

	llist_destroy(l);

	return 0;
}