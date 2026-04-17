#ifndef POOL_H
#define POOL_H

#define _GNU_SOURCE
#include <pthread.h>
#include "queue.h"

#define MAX_JOB 10     // 任务队列最大容量
#define MIN_FREE_NR 3  // 最小空闲线程数 (初始线程数)
#define MAX_FREE_NR 5  // 最大空闲线程数 (超过则销毁多余线程)
#define STEP 2         // 每次动态调整线程数量的步长

// 线程池结构体
typedef struct
{
	pthread_t *workers;    // 工作线程 ID 数组 (动态分配)
	pthread_t admin_tid;   // 管理者线程 ID
	queue_t *task_queue;   // 任务队列 (存放 task_t)

	int max_threads;       // 线程池允许的最大线程数
	int min_free_threads;  // 最小空闲线程数 (池中常驻线程数)
	int max_free_threads;  // 最大空闲线程数 (超过则销毁多余线程)
	int busy_threads;      // 当前正在执行任务的线程数
	int live_threads;      // 当前存活的线程总数
	int exit_threads;      // 需要退出的线程数 (由管理者设置)
	int shutdown;          // 线程池关闭标志: 0-运行中; 1-正在关闭
	pthread_mutex_t mutex_pool;      // 整个线程池的互斥锁, 保护线程池核心数据 (任务队列、状态变量等)
	pthread_mutex_t mutex_busy;      // 专门保护 busy_threads 的互斥锁
	pthread_cond_t queue_not_empty;  // 任务队列非空时唤醒工作线程, 通知取任务
	pthread_cond_t queue_not_full;   // 任务队列未满时唤醒添加任务的线程, 通知添加新任务
}pool_t;

// 任务结构
typedef struct
{
	void *(*job)(void *s);  // 任务函数指针
	void *arg;              // 任务函数参数
}task_t;


/*
* 初始化线程池
* @param mypool 输出参数, 创建成功的线程池地址回填
* @param capacity 最大线程容量
* @return 成功返回 0; 失败返回 <0
*/
extern int pool_init(pool_t **mypool, int capacity);

/*
* 添加任务到线程池
* @param mypool 线程池指针
* @param t 要添加的任务
* @return 成功返回 0; 失败返回 <0
*/
extern int pool_add_task(pool_t *mypool, const task_t *t);


/*
* 销毁线程池, 释放所有资源
* @param mypool 要销毁的线程池指针
* @return 空
*/
extern void pool_destroy(pool_t *mypool);

#endif