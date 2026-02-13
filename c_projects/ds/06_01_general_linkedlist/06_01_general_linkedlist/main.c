/*
	通用性链表 01
	有头双向循环链表
*/
#include <stdio.h>
#include <string.h>
#include "llist.h"

// 调试用
#define EXECUTE 0

#define NAMESIZE 20

struct student  // 客户结构体
{
	int id;
	char name[NAMESIZE];
	int tel;
};

void print_student(const void *list_data)  // 客户提供的打印方法
{
	const struct student *stu = (const struct student *)list_data;  // 转换成 struct student * 结构体类型的指针 
	printf("id = %d, name = %s, tel = %d\n", stu->id, stu->name, stu->tel);
}

int id_cmp(const void *list_data, const void *search_key)  // 客户提供的对比方法
{
	const struct student *stu = (const struct student *)list_data;
	const int *target_id = (const int *)search_key;
	return stu->id == *target_id;
}

int name_cmp(const void *list_data, const void *search_key)
{
	const struct student *stu = (const struct student *)list_data;
	const char *target_name = (const char *)search_key;
	return !strcmp(stu->name, target_name);
}

int tel_cmp(const void *list_data, const void *search_key)
{
	const struct student *stu = (const struct student *)list_data;
	const int *target_tel = (const int *)search_key;
	return stu->tel == *target_tel;
}

int main(void)
{
	LLIST *handler = llist_create(sizeof(struct student));
	if (!handler)
	{
		printf("Create llist failed\n");
		return -1;
	}

	struct student stu;  // 储存客户录入的数据
	for (int i = 0; i < 5; i++)
	{
		stu.id = 100 + i;
		sprintf(stu.name, "stu%d", i);
		stu.tel = 10010 + i;
		llist_insert(handler, &stu, TAILINSERT);  // 插入数据节点
	}

	llist_display(handler, print_student);  // 遍历链表

	printf("----------------------------------\n");

	int find_id = 100;  // 模拟要查找的 ID 号
	char *find_name = "stu4";  // 模拟要查找的名字

#if EXECUTE == 0
	struct student *find = (struct student *)llist_find(handler, find_name, name_cmp);
	if(!find)
		printf("Not found: name = %s\n", find_name);
	else
		printf("Found: id = %d, name = %s, tel = %d\n", find->id, find->name, find->tel);

#elif EXECUTE == 1
	struct student save;  // save 变量储存取出节点的数据
	int ret = llist_fetch(handler, &find_id, id_cmp, &save);
	if (ret != 0)
		printf("Fetch failed: id = %d\n", find_id);
	else
		printf("Fetch ok: id = %d, name = %s, tel = %d\n", save.id, save.name, save.tel);

#elif EXECUTE == 2
	int ret = llist_delete(handler, &find_id, id_cmp);
	if (ret != 0)
		printf("Delete failed: id = %d\n", find_id);
	else
		printf("Delete ok: id = %d\n", find_id);

#endif
	printf("----------------------------------\n");

	llist_display(handler, print_student);  // 遍历链表

	llist_destroy(handler);  // 销毁链表

	return 0;
}