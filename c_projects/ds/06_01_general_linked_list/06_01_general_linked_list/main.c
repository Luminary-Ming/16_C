#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

#define NAMESIZE 20

struct stu  // 客户提供的结构体结构
{
	int id;
	char name[NAMESIZE];
	int math;
	int tel;
};

void print(const void *data)  // 客户提供的打印方法
{
	const struct stu *p = data;  // 转换成 struct stu * 结构体类型的指针
	printf("%d  %s  %d  %d\n", p->id, p->name, p->math, p->tel);
}

int id_cmp(const void *data, const void *key)  // 客户提供的对比数据方法
{
	const struct stu *p = data;
	const int *k = key;  // 转换成 int * 类型的指针
	return !(p->id - *k);
}

int tel_cmp(const void *data, const void *key)  // 客户提供的对比数据方法
{
	const struct stu *p = data;
	const int *k = key;  // 转换成 int * 类型的指针
	return !(p->tel - *k);
}

int name_cmp(const void *data, const void *key)  // 客户提供的对比数据方法
{
	const struct stu *p = data;
	const char *k = key;  // 转换成 char * 类型的指针
	return !strcmp(p->name, k);
}


int main(void)
{
	LLIST *handler = NULL;  // 指向头节点
	struct stu data;  // 储存客户录入的数据
	struct stu save;  // 储存取出的数据
	struct stu *find = NULL;  // 指向要查找的数据

	int find_id = 100;
	char *find_name = "stu4";
	int find_tel = 10012;

	handler = llist_create(sizeof(struct stu));  // 创建头节点
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
		data.tel = 10010 + i;
		llist_insert(handler, &data, TAILINSERT);
	}

	llist_display(handler, print);

	printf("------------------------------------\n");

	find = llist_find(handler, &find_tel, tel_cmp);
	if (find == NULL)
		printf("Not Find It!\n");
	else
	{
		printf("Find It!\n");
		print(find);
	}

	int ret = llist_delete(handler, &find_tel, tel_cmp);
	if (ret != 0)
		printf("Delete It Is Failed!\n");
	else
		printf("Delete It!\n");

	ret = llist_fetch(handler, &find_tel, tel_cmp, &save);
	if (ret != 0)
		printf("Fetch It Is Failed!\n");
	else
	{
		printf("Fetch It!\n");
		print(&save);
	}

	printf("------------------------------------\n");

	llist_display(handler, print);

	llist_destroy(handler);

	return 0;
}