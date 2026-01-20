#include <stdio.h>
#include "llist.h"

int main(void)
{
	LLIST *handler = NULL;  // handler 指针指向头节点
	LLIST *find = NULL;  // find 指针指向找到的节点
	LLIST data;  // data 变量存储用户输入的数据
	LLIST save;  // save 变量存储取出的数据
	int ret = 0;
	
	int find_id = 100;
	char *find_name = "stu4";

	handler = llist_create();  // 创建头节点
	if (handler == NULL)
	{
		fprintf(stderr, "llist_create() Is Failed!\n");
		return -1;
	}

	for (int i = 0; i < 5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name, "stu%d", i);
		data.math = 100 - i;
		data.prev = data.next = NULL;
		llist_insert(handler, &data, TAILINSERT);  // 插入数据节点
	}

	llist_display(handler);  // 遍历链表

	printf("----------------------------\n");
#if 0
	find = llist_find(handler, find_name);  // 查找节点
	if(find == NULL)
		printf("Not Find It!\n");
	else
		printf("Find It! %d  %s  %d\n", find->id, find->name, find->math);
#elif 1
	ret = llist_delete(handler, &find_id);  // 删除节点
	if (ret != 0)
		printf("Delete It Is Failed!\n");
	else
		printf("Delete It!\n");
#elif 0
	ret = llist_fetch(handler, find_name, &save);
	if (ret != 0)
		printf("Fetch It Is Failed!\n");
	else
		printf("Fetch It! %d  %s  %d\n", save.id, save.name, save.math);
#else
#endif
	printf("----------------------------\n");

	llist_display(handler);  // 遍历链表

	llist_destroy(handler);  // 销毁链表

	return 0;
}