/*
	哈希表
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

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
	printf("%d  %s  %d  %d->", p->id, p->name, p->math, p->tel);
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

/*
	功能 : 基于学生 ID 的哈希函数, 计算数据应存入的索引值
	参数 : data     指向学生结构体的指针
		   capacity 哈希表的容量
  返回值 : 计算出哈希索引
*/
unsigned int id_hash(const void *data, int capacity);

/*
	功能 : 基于学生 ID 的哈希函数, 计算数据应存入的索引值
	参数 : data     指向学生结构体的指针
		   capacity 哈希表的容量
  返回值 : 计算出哈希索引
*/
unsigned int tel_hash(const void *data, int capacity);

/*
	功能 : 基于学生 name 的哈希函数, 计算数据应存入的索引值
	参数 : data     指向学生结构体的指针
		   capacity 哈希表的容量
  返回值 : 计算出哈希索引
*/
unsigned int name_hash(const void *data, int capacity);

int main(void)
{
	HTABLE *ht = htable_create(sizeof(struct stu), 8);  // 假设客户指定哈希表容量为 8
	if (!ht)  // 如果创建失败
	{
		fprintf(stderr, "htable_create is failed!\n");
		return -1;
	}

	struct stu data;  // 存储客户录入的数据
	for (int i = 0; i < 10; i++)  // 循环录入数据
	{
		data.id = 100 + i;
		sprintf(data.name, "stu%d", i);
		data.math = 100 - i;
		data.tel = 10086 + i;
		// htable_insert(指向哈希表的指针, 插入的数据, 哈希函数, 对比函数, 插入模式)
		htable_insert(ht, &data, id_hash, id_cmp,HASH_TABLE_INSERT_REPLACE);
	}

	htable_display(ht, print);  // 遍历哈希表

	printf("--------------------------------------\n");

	int find_id = 102;
	char *find_name = "stu8";
	int find_tel= 10088;
#if 0
	struct stu *find = htable_find(ht, &find_id, id_hash, id_cmp);
	if(!find)
		printf("Not Find It!\n");
	else
	{
		printf("Find It!\n");
		print(find);
	}
#elif 0
	int ret = htable_delete(ht, &((int) { 104 }), id_hash, id_cmp);
	if (ret != 0)
		printf("Delete It Is Failed!");
	else
		printf("Delete It!");

#elif 1
	struct stu save;
	int ret = htable_fetch(ht, &find_tel, id_hash, tel_cmp, &save, &find_id);
	if (ret != 0)
		printf("Fetch It Is Failed!");
	else
	{
		printf("Fetch It!\n");
		print(&save);
	}

#else
#endif

	printf("\n--------------------------------------\n");

	htable_display(ht, print);  // 遍历哈希表

	htable_destroy(ht);  // 销毁哈希表

	return 0;
}

/*
	  功能 : 基于学生 ID 的哈希函数, 计算数据应存入的索引值
	  参数 : data 指向学生结构体的指针
			 capacity 哈希表的容量
	返回值 : 计算出哈希索引
*/
unsigned int id_hash(const void *data, int capacity)
{
	const struct stu *p = data;  // 转换指针(void * 类型转成 struct stu * 类型)
	return p->id % capacity;  // 返回哈希索引
}

unsigned int tel_hash(const void *data, int capacity)
{
	const struct stu *p = data;  // 转换指针(void * 类型转成 struct stu * 类型)
	return p->tel % capacity;  // 返回哈希索引
}

unsigned int name_hash(const void *data, int capacity)
{
	const char *name = data;
	unsigned int hash = 0;  // 存储哈希索引值
	while (*name)
	{
		// 字符串哈希函数 hash * 31 + 字符 ACII 值
		hash = hash * 31 + *name++;
	}
	return hash % capacity;
}
/*
	为什么是 hash = hash * 31 + *name++; hash % capacity; 来获取哈希索引

	如果是 hash = hash + *name++;  简单累加, 只是把 ASCII 值加起来
	那么像 "abc" 和 "cba" 会得到相同哈希值
	'a'+'b'+'c' = 97+98+99 = 294
	'c'+'b'+'a' = 99+98+97 = 294  ← 会产生哈希冲突!

	使用乘法：
	hash = hash * 31 + *name++;
	为什么是 31 ?
	数学特性：
	1. 质数：减少冲突概率
	2. 奇数：避免偶数乘数的模式
	3. 接近 2 的幂：31 = 32 - 1

	优化：
	hash * 31 = (hash << 5) - hash   移位比乘法快很多!

    unsigned int hash = 0;
	const char *name = "abc";  // a=97, b=98, c=99

	// 第一次循环：name指向 'a'
	hash = 0 * 31 + 97 = 97

	// 第二次循环：name指向 'b'
	hash = 97 * 31 + 98 = 3007 + 98 = 3105

	// 第三次循环：name指向 'c'
	hash = 3105 * 31 + 99 = 96255 + 99 = 96354

	最终哈希值：96354
	第二阶段：映射到哈希表范围
	hash % capacity;  // 缩放到 0 到 capacity-1
	96354 % 8 = 2  哈希索引是 2
*/



/*
哈希表结构体 (struct htable_head)
┌─────────────────────────────────────┐
│ size = sizeof(struct stu)           │
├─────────────────────────────────────┤
│ capacity = 8                        │
├─────────────────────────────────────┤
│ bucket ─────────────────────────────┼─────────┐
└─────────────────────────────────────┘         │
												⬇️
									 [指针数组] (LLIST *数组)
									 ┌───────┬───────┬───────┬───────┬───┐
									 │ ptr0  │ ptr1  │ ptr2  │ ptr3  │...│
									 └───┬───┴───┬───┴───┬───┴───┬───┴───┘
										 │       │       │       │
										 ⬇️       ⬇️       ⬇️       ⬇️
									  ┌──────┐┌──────┐┌──────┐┌──────┐
		         (struct llist_head)  │ size ││ size ││ size ││ size │
									  ├──────┤├──────┤├──────┤├──────┤
									  │ head ││ head ││ head ││ head │
									  └───┬──┘└──────┘└──────┘└──────┘
										  │               
										  │               
					(struct llist_node)	  ⬇️   
									 ┌─────────┐          
									 │  prev   │          
									 ├─────────┤                   
									 │  next ──┼────┐
									 ├─────────┤    │
									 │ id=100  │   	│
									 │ name=   │    │
									 │ "stu0"  │    │
									 │ math=99 │    │
									 │ tel=    │    │
									 │ 10086   │    │
									 └─────────┘  	│	
												    │
												    ⬇️
												 ┌─────────┐
												 │  prev   │
												 ├─────────┤
												 │  next ──┼────┐
												 ├─────────┤    │
												 │ id=101  │   	│
												 │ name=   │    │
												 │ "stu1"  │    │
												 │ math=98 │    │
												 │ tel=    │    │
												 │ 10087   │    │
												 └─────────┘  	│
																│
																⬇️
                                                                ...
*/															   