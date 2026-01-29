#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H

#define HASH_TABLE_DEFAULT_CAPACITY 10  // 定义哈希表的默认容量
#define HASH_TABLE_INSERT_REPLACE 0  // 覆盖已存在数据的标识
#define HASH_TABLE_INSERT_NOSAME 1  // 不允许重复数据的标识
#define HASH_TABLE_INSERT_SAME 2  // 允许重复数据的标识

typedef void (*htable_op)(const void *);  // 定义函数指针, 用于打印哈希表
typedef int (*htable_cmp)(const void *, const void *);  // 用于比较哈希表的数据
typedef unsigned int (*htable_hash)(const void *, int);  // 用于计算哈希索引

typedef void HTABLE;  // 定义哈希表结构 (隐藏底层结构体的实现)

HTABLE *htable_create(int size, int capacity);

/*
	功能: 往哈希表中插入数据
	参数: handler 指向哈希表结构的指针
	      data    指向要插入的数据
		  hash    存储使用的哈希函数
		  cmp     存储使用的对比函数
		  mode    存储插入模式
  返回值: 成功返回 0, 失败返回 <0
*/
int htable_insert(HTABLE* handler, const void *data, htable_hash hash, htable_cmp cmp, int mode);

/*
	功能: 遍历数据
	参数: handler 指向哈希表结构的指针
		  op      打印方法
  返回值: 空
*/
void htable_display(HTABLE *handler, htable_op op);

/*
	功能: 销毁数据
	参数: handler 指向哈希表结构的指针
  返回值: 空
*/
void htable_destroy(HTABLE *handler);

/*
	功能: 查找数据
	参数: handler   指向哈希表结构的指针
		  find_data 指向要插入的数据
		  hash      存储使用的哈希函数
		  cmp       存储使用的对比函数
  返回值: 成功返回查到的数据节点的数据域地址, 失败返回 NULL
*/
void *htable_find(HTABLE *handler, const void *find_data, htable_hash hash, htable_cmp cmp);


/*
	功能: 删除数据
	参数: handler   指向哈希表结构的指针
		  find_data 指向要插入的数据
		  hash      存储使用的哈希函数
		  cmp       存储使用的对比函数
  返回值: 成功返回 0, 失败返回 -1
*/
int htable_delete(HTABLE *handler, const void *find_data, htable_hash hash, htable_cmp cmp);

/*
	功能: 取出数据
	参数: handler   指向哈希表结构的指针
		  find_data 指向要插入的数据
		  hash      存储使用的哈希函数
		  cmp       存储使用的对比函数
		  save      存储取出的数据
  返回值: 成功返回 0, 失败返回 -1
*/
int htable_fetch(HTABLE *handler, const void *find_data, htable_hash hash, htable_cmp cmp, void *save, void *id);

#endif