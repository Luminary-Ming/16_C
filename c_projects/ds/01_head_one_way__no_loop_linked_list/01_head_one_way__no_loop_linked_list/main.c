/*
	有头单向不循环链表
	有头 : 拥有头节点
	单向 : 链表中每一个节点都只有一个指向下一个节点的指针
	不循环 : 链表中的最后一个节点指向 NULL
*/
#include <stdio.h>
#include "llist.h"

#define EXECUTE 1

int main(void)
{
	LLIST *handler = NULL; // handler 指针指向链表的头节点
	LLIST data;  // data 变量存储录入的数据
	LLIST save;  // save 变量储存取出节点的数据
	LLIST *find = NULL;  // find 指针指向查找到的节点
	int find_id = 100;  // 要查找的 ID 号
	char *find_name = "stu4";  // 要查找的名字
	int ret = 0;  // 储存返回值

	handler = llist_create();  // 创建头节点
	if (handler == NULL)
	{
		fprintf(stderr, "llist_create() Is Failed!\n");
		return -1;  // 创建头节点失败, 结束程序, 返回 -1 错误码
	}

	for (int i = 0; i < 5; i++)
	{
		data.id = 100 + i;  // 录入数据
		sprintf(data.name, "stu%d", i);  // 录入数据
		data.math = 100 - i;  // 录入数据
		data.next = NULL;  // 初始指针指向 NULL
		llist_insert(handler, &data, TAILINSERT);  // 插入数据节点
	}

	llist_display(handler);  // 遍历链表

	printf("----------------------------------\n");
#if EXECUTE == 0
	find = llist_find(handler, find_name);  // 查找数据节点
	if(find == NULL)  
		printf("Not Find It!\n");
	else
		printf("Find It! %d  %s  %d\n", find->id, find->name, find->math);

#elif EXECUTE == 1
	ret = llist_delete(handler, &find_id);  // 删除数据节点
	if (ret != 0)  //  判断删除数据节点是否失败
		printf("Delete It Is Failed!\n");
	else
		printf("Delete It!\n");

#elif EXECUTE == 2
	ret = llist_fetch(handler, &find_id, &save);  // 取出节点的数据
	if(ret != 0)
		printf("Fetch It Is Failed!\n");
	else
		printf("Fetch It! %d  %s  %d\n", save.id, save.name, save.math);
	
#endif

	printf("----------------------------------\n");

	llist_display(handler);  // 遍历链表

	llist_destroy(handler);  // 销毁链表

	return 0;
}