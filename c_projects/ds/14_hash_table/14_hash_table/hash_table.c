#include "hash_table.h"
#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct htable_head  // 哈希表核心结构体(对外隐藏, 仅内部可见, 为了保证封装性)
{
	int size;  // 单个数据节点的数据域大小
	int capacity;  // 哈希表的容量(数组的长度)
	LLIST **bucket;  // 桶数组指针(每个元素是一个链表 LLIST * 的头节点)
	/*
		LLIST **bucket, bucket 指向 LLIST * 类型的指针数组
		易混点: 
		LLIST *pointer_array[3];   // 有一个指针数组
		LLIST *p = pointer_array;  // ❌️ 编译器会警告，但假设我们强制转换 ( 因为 p 的类型是 LLIST *, 指向 LLIST 对象的指针 )
		LLIST **p = pointer_array; // ✔️ 正确, p 的类型是 LLIST **, 指向 LLIST * 指针的指针
	*/                			
};


HTABLE *htable_create1(int size, int capacity)
{
	if (size <= 0)  // 判断参数是否合法
		return NULL;  // 由于参数不合法, 结束函数, 返回 NULL

	if (capacity <= 0)  // 判断参数是否合法
		capacity = HASH_TABLE_DEFAULT_CAPACITY;  // 由于参数不合法, 使用默认容量

	struct htable_head *handler = malloc(sizeof(struct htable_head));  // 开辟哈希表结构的空间
	if (!handler)  // 判断开辟哈希表结构的空间是否失败 
		return NULL;  // 开辟失败, 结束函数, 返回 NULL

	handler->size = size;  // 存储客户指定的数据节点中的数据域大小
	handler->capacity = capacity;  // 存储最终的容量大小
	handler->bucket = malloc(sizeof(LLIST *) * capacity);  // 分配桶数组的空间
	if (!handler->bucket)  // 判断分配桶数组的空间是否失败
	{
		free(handler);  // 释放哈希表结构的空间
		return NULL;  // 由于分配桶数组的空间失败, 分配桶数组的空间
	}

	for (int i = 0; i < capacity; i++)  // 循环为每个桶创建一个空链表(头节点)
	{
		handler->bucket[i] = llist_create(size);  // 为第 i 个桶创建头节点
		if (!handler->bucket[i])  // 判断第 i 个桶创建头节点是否失败
		{
			for (int j = 0; j < i; j++)  // 回滚, 销毁之前创建成功的所有桶
			{
				llist_destroy(handler->bucket[j]);  // 销毁链表
			}
			free(handler->bucket);  // 释放桶数组的空间
			free(handler);  // 释放哈希表结构的空间
			return NULL;  // 由于为第 i 个桶创建头节点失败, 结束函数, 返回 NULL
		}
	}
	return handler;
}

HTABLE *htable_create(int size, int capacity)
{
	if (size <= 0)  // 判断参数是否合法
		goto ERR_1;

	if (capacity <= 0)  // 判断参数是否合法
		capacity = HASH_TABLE_DEFAULT_CAPACITY;  // 由于参数不合法, 使用默认容量

	struct htable_head *handler = malloc(sizeof(struct htable_head));  // 开辟哈希表结构的空间
	if (!handler)  // 判断开辟哈希表结构的空间是否失败 
		goto ERR_1;

	handler->size = size;  // 存储客户指定的数据节点中的数据域大小
	handler->capacity = capacity;  // 存储最终的容量大小
	handler->bucket = malloc(sizeof(LLIST *) * capacity);  // 分配桶数组的空间
	if (!handler->bucket)  // 判断分配桶数组的空间是否失败
		goto ERR_2;

	for (int i = 0; i < capacity; i++)  // 循环为每个桶创建一个空链表(头节点)
	{
		handler->bucket[i] = llist_create(size);  // 为第 i 个桶创建头节点
		if (!handler->bucket[i])  // 判断第 i 个桶创建头节点是否失败
		{
			for (int j = 0; j < i; j++)  // 回滚, 销毁之前创建成功的所有桶
			{
				llist_destroy(handler->bucket[j]);  // 销毁链表
			}
			goto ERR_3;
		}
	}

	return handler;
ERR_3:
	free(handler->bucket);  // 释放桶数组的空间
ERR_2:
	free(handler);  // 释放哈希表结构的空间
ERR_1:
	return NULL;
}

int htable_insert(HTABLE *handler, const void *data, htable_hash hash, htable_cmp cmp, int mode)
{
	if (!handler || !data || !hash || !cmp)  // 判断参数是否合理
		return -1;  // 由于参数不合理, 结束函数, 返回 -1

	struct htable_head *h = handler;  // 转换指针

	unsigned int idx = hash(data, h->capacity);  // 使用客户指定的哈希函数计算索引值

	switch (mode)  // 选择插入模式
	{
		case HASH_TABLE_INSERT_REPLACE:  // 覆盖已存在数据的标识
			llist_delete(h->bucket[idx], data, cmp);  // 在对应的桶中删除重复的数据
			break;

		case HASH_TABLE_INSERT_NOSAME:  // 不允许重复数据的标识
			void *find_data = llist_find(h->bucket[idx], data, cmp);
			if (find_data)  // 判断是否已存在相同数据
				return -2;  // 已存在相同数据不插入, 结束函数, 返回 -2

		case HASH_TABLE_INSERT_SAME:  // 允许重复数据的标识
			break; 
		default: return -3;  // 由于插入模式失败, 结束函数, 返回 -3
	}

	return llist_insert(h->bucket[idx], data, TAILINSERT);  // 将数据以尾插的形式插入到对应的桶中
}

void htable_display(HTABLE *handler, htable_op op)
{
	if (!handler || !op)  // 判断参数是否合理
		return;  // 由于参数不合理, 结束函数

	struct htable_head *h = handler;  // 转换指针

	for (int i = 0; i < h->capacity; i++)  // 循环遍历哈希表桶
	{
		printf("BUCKET[%d] : ", i); // 打印哈希表桶的编号
		llist_display(h->bucket[i], op);  // 遍历下标为i的链表
		printf("\n");  // 换行
	}
}

void htable_destroy(HTABLE *handler)
{
	if (!handler)  // 判断参数是否合理
		return;  // 由于参数不合理, 结束函数

	struct htable_head *h = handler;  // 转换指针

	for (int i = 0; i < h->capacity; i++)  // 循环销毁哈希表桶中每条链表
	{
		llist_destroy(h->bucket[i]);
		h->bucket[i] = NULL;
	}	
	free(h->bucket);  // 释放哈希表桶
	h->bucket = NULL;
	free(h);  // 释放哈希表结构
	h = NULL;
}

static int _find(HTABLE *handler, const void *find_data, htable_hash hash, htable_cmp cmp, void *id)
{
	if (!handler || !find_data || !hash || !cmp)  // 判断参数是否合理
		return -1;  // 由于参数不合理, 结束函数, 返回 -1

	struct htable_head *h = handler;  // 转换指针

	unsigned int idx = hash(id, h->capacity);  // 使用客户指定的哈希函数计算索引值

	return idx;
}
#if 0
void *htable_find(HTABLE *handler, const void *find_data, htable_hash hash, htable_cmp cmp)
{
	int idx = _find(handler, find_data, hash, cmp);
	if (idx < 0)
		return NULL;

	struct htable_head *h = handler;  // 转换指针

	return llist_find(h->bucket[idx], find_data, cmp);
}

int htable_delete(HTABLE *handler, const void *find_data, htable_hash hash, htable_cmp cmp)
{
	int idx = _find(handler, find_data, hash, cmp);
	if (idx < 0)
		return -1;

	struct htable_head *h = handler;  // 转换指针

	return llist_delete(h->bucket[idx], find_data, cmp);
}
#else
#endif

int htable_fetch(HTABLE *handler, const void *find_data, htable_hash hash, htable_cmp cmp, void *save, void*id)
{
	int idx = _find(handler, find_data, hash, cmp, id);
	if (idx < 0)
		return -1;

	struct htable_head *h = handler;  // 转换指针

	return llist_fetch(h->bucket[idx], find_data, cmp, save);
}