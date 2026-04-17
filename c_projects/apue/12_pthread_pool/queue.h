/*
	任务队列
*/
#ifndef QUEUE_H
#define QUEUE_H

typedef struct
{
	void *data;  // 数据域
	int front;   // 对头标记
	int rear;    // 队尾标记
	int capacity;  // 容量
	int size;  // 大小
}queue_t; // 命名 queue_t (queue_type, 与 size_t, time_t, int32_t 等等有异曲同工之妙)

/*
	功能: 队列初始化
	参数: q 回填开辟出的地址
		  capacity 客户指定的容量
		  size 客户指定一个空间的大小
  返回值: 成功返回 0, 失败返回 < 0
*/
int queue_init(queue_t **q, int capacity, int size);

/*
	功能: 判断队列是否为空
	参数: q 指向要判断的队列
  返回值: 为空返回 1, 不为空返回 0
*/
int queue_is_full(const queue_t *q);

/*
	功能: 判断队列是否为满
	参数: q 指向要判断的队列
  返回值: 为满返回 1, 不为满返回 0
*/
int queue_is_empty(const queue_t *q);

/*
	功能: 入队
	参数: q 指向要入队的队列
		  data 指向要入队的数据
  返回值: 成功返回 0,失败返回 < 0
*/
int queue_en(queue_t *q, const void *data);

/*
	功能: 出队
	参数:    q 指向要出队的队列
		  save 指向存储空间(存储出队的数据)
  返回值: 成功返回 0, 失败返回 < 0
*/
int queue_de(queue_t *q, void *save);

/*
	功能: 释放队列
	参数: q 指向要释放的队列
  返回值: 空
*/
void queue_destroy(queue_t *q);

#endif