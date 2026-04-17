#define _GNU_SOURCE
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <signal.h> 
#include <string.h> 
#include <unistd.h>
#include "pool.h"

static void *admin_job(void *arg);   // 管理者线程函数声明
static void *worker_job(void *arg);  // 工作线程函数声明

int pool_init(pool_t **mypool, int capacity)
{
	pool_t *me = malloc(sizeof(pool_t));  // 开辟线程池结构体内存
	if (me == NULL)
		return -1;

	// 开辟工作线程 ID 数组, capacity 个元素, 初始化为 0 (calloc 特性)
	me->workers = calloc(capacity, sizeof(pthread_t));  
	if (me->workers == NULL)
	{
		free(me);
		return -2;
	}

	// 初始化任务队列, 最大容量 MAX_JOB
	queue_init(&me->task_queue, MAX_JOB, sizeof(task_t));

	// 设置线程池参数
	me->max_threads = capacity;              // 最大线程容量
	me->min_free_threads = MIN_FREE_NR;      // 最小空闲线程数 (常驻线程)
	me->max_free_threads = MAX_FREE_NR;      // 最大空闲线程数
	me->busy_threads = 0;                    // 初始没有忙碌线程
	me->live_threads = me->min_free_threads; // 初始存活线程 = 最小空闲数
	me->exit_threads = 0;                    // 初始没有需要退出的线程
	me->shutdown = 0;                        // 关闭标志为 0

	// 初始化互斥锁和条件变量
	pthread_mutex_init(&me->mutex_pool, NULL);  
	pthread_mutex_init(&me->mutex_busy, NULL);  
	pthread_cond_init(&me->queue_not_empty, NULL);  
	pthread_cond_init(&me->queue_not_full, NULL);  

	// 创建管理者线程
	int ret = pthread_create(&me->admin_tid, NULL, admin_job, me);
	if (ret != 0)
	{
		free(me->workers);
		free(me);
		return -ret;
	}

	// 创建初始的工作线程 (min_free_threads 个)
	for (int i = 0; i < me->min_free_threads; i++)
	{
		ret = pthread_create(me->workers + i, NULL, worker_job, me);
		if (ret != 0)
		{
			free(me->workers);
			free(me);
			return -ret;
		}

		// 将工作线程设置为分离状态, 线程退出时自动回收资源 (后续 pool_destroy() 中不需要调用 pthread_join() 去等待工作线程退出)
		pthread_detach((me->workers)[i]);
	}

	*mypool = me;  // 将创建好的线程池返回给调用者
	return 0;
} 

// 查找空闲槽位 (用于创建新线程)
static int _get_free_pos(pthread_t *jobs, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (jobs[i] == 0)  // 槽位值为 0 表示从未使用过 (calloc 初始化为 0)
			return i;

		if (pthread_kill(jobs[i], 0) == ESRCH)  // 已退出的线程槽位 (pthread_kill 返回 ESRCH 表示线程已退出)
			return i;
	}

	return -1;  // 没有找到空闲槽位
}

// 管理者线程函数: 动态调整线程数量、输出状态
static void *admin_job(void *arg)
{
	pool_t *mypool = arg;

	int busy_count = 0;
	int free_count = 0;

	while (1)
	{
		// 锁定线程池主锁, 保护 live_threads、shutdown 等变量
		pthread_mutex_lock(&mypool->mutex_pool);

		// 如果线程池关闭, 管理者线程退出
		if (mypool->shutdown == 1)
		{
			pthread_mutex_unlock(&mypool->mutex_pool);
			break;
		}

		// 使用独立的 busy 锁, 获取当前忙碌线程数
		pthread_mutex_lock(&mypool->mutex_busy);
		busy_count = mypool->busy_threads;  
		pthread_mutex_unlock(&mypool->mutex_busy);

		// 计算空闲线程数 = 存活线程 - 忙碌线程
		free_count = mypool->live_threads - busy_count;

		// 如果空闲线程过多 (超过最大空闲数 + STEP), 则通知 STEP 个线程退出
		if (free_count >= mypool->max_free_threads + STEP)
		{
			mypool->exit_threads = STEP;  // 设置需要退出的线程数
			for (int i = 0; i < STEP; i++)
				pthread_cond_signal(&mypool->queue_not_empty);  // 唤醒工作线程让其退出
		}

		// 如果所有线程都忙碌且未达到最大线程数, 则创建新线程
		if (busy_count == mypool->live_threads && mypool->live_threads < mypool->max_threads)
		{
			for (int i = 0; i < STEP; i++)  // 每次最多增加 STEP 个线程
			{
				int pos = _get_free_pos(mypool->workers, mypool->max_threads);  // 获取空槽位
				if (pos == -1)
				{
					fprintf(stderr, "[%d] _get_free_pos() is failed\n", __LINE__);
					break;
				}

				// 创建新工作线程
				int ret = pthread_create(mypool->workers + pos, NULL, worker_job, mypool);
				if (ret != 0)
				{
					fprintf(stderr, "pthread_create failed: %s\n", strerror(ret));
					break;
				}

				pthread_detach((mypool->workers)[pos]);  // 分离新线程
				mypool->live_threads++;  // 存活线程数加1
			}
		}

		// 输出线程池状态: O 表示忙碌线程, X 表示空闲线程
		pthread_mutex_lock(&mypool->mutex_busy);
		int busy = mypool->busy_threads;  // 当前忙碌线程数
		pthread_mutex_unlock(&mypool->mutex_busy);

		int live = mypool->live_threads;  // 当前存活线程数

		for (int i = 0; i < busy; i++)
			write(1, "O", 1);
		for (int i = 0; i < live - busy; i++)
			write(1, "X", 1);
		write(1, "\n", 1);

		pthread_mutex_unlock(&mypool->mutex_pool);  // 释放线程池主锁
		sleep(1);  // 每秒检查一次
	}
	pthread_exit(0);  // 管理者线程退出
}

// 工作线程函数: 从任务队列取任务并执行
static void *worker_job(void *arg)
{
	pool_t *mypool = arg;
	task_t mytask;

	while (1)
	{
		pthread_mutex_lock(&mypool->mutex_pool);  // 锁定线程池主锁

		// 等待条件: 队列不为空, 且线程池未关闭, 且没有收到退出通知
		while (queue_is_empty(mypool->task_queue) && mypool->shutdown == 0 && mypool->exit_threads == 0)
			pthread_cond_wait(&mypool->queue_not_empty, &mypool->mutex_pool);

		// 如果线程池关闭, 工作线程退出
		if (mypool->shutdown == 1)
		{
			pthread_mutex_unlock(&mypool->mutex_pool);  // 释放线程池主锁
			break;
		}

		// 如果收到退出通知 (管理者要求销毁线程)
		if (mypool->exit_threads > 0)
		{
			mypool->exit_threads--;  // 减少待退出计数
			mypool->live_threads--;  // 存活线程数减 1
			pthread_mutex_unlock(&mypool->mutex_pool);
			break;  // 线程退出
		}

		// 从任务队列取出一个任务
		queue_de(mypool->task_queue, &mytask);

		// 通知可能正在等待队列非满的添加任务线程
		pthread_cond_signal(&mypool->queue_not_full);
		pthread_mutex_unlock(&mypool->mutex_pool);  // 释放线程池主锁

		// 增加忙碌线程计数
		pthread_mutex_lock(&mypool->mutex_busy);
		mypool->busy_threads++;
		pthread_mutex_unlock(&mypool->mutex_busy);

		// 执行任务函数
		(mytask.job)(mytask.arg);

		// 减少忙碌线程计数
		pthread_mutex_lock(&mypool->mutex_busy);
		mypool->busy_threads--;
		pthread_mutex_unlock(&mypool->mutex_busy);
	}
	pthread_exit(0);  // 工作线程退出
}

int pool_add_task(pool_t *mypool, const task_t *t)
{
	pthread_mutex_lock(&mypool->mutex_pool);  // 锁定线程池主锁

	// 如果线程池已关闭, 拒绝添加新任务
	if (mypool->shutdown == 1)
	{
		pthread_mutex_unlock(&mypool->mutex_pool);
		return -1;
	}

	// 如果任务队列已满, 等待队列非满信号
	while (queue_is_full(mypool->task_queue))
		pthread_cond_wait(&mypool->queue_not_full, &mypool->mutex_pool);

	// 再次检查关闭标志 (可能在等待期间线程池被关闭)
	if (mypool->shutdown == 1)
	{
		pthread_mutex_unlock(&mypool->mutex_pool);
		return -1;
	}

	// 将任务加入队列
	queue_en(mypool->task_queue, t);

	// 通知等待任务的工作线程
	pthread_cond_signal(&mypool->queue_not_empty);
	pthread_mutex_unlock(&mypool->mutex_pool);  // 释放线程池主锁

	return 0;
}

void pool_destroy(pool_t *mypool)
{
	// 设置关闭标志, 并唤醒所有等待任务的工作线程
	pthread_mutex_lock(&mypool->mutex_pool);
	mypool->shutdown = 1;
	pthread_cond_broadcast(&mypool->queue_not_empty);
	pthread_mutex_unlock(&mypool->mutex_pool);

	// 等待管理者线程结束
	pthread_join(mypool->admin_tid, NULL);
	// 管理者线程使用 pthread_join, 工作线程使用 pthread_detach
	// 管理者线程数量固定 (只有1个), pthread_join 开销可忽略
	// 需要确保管理者线程已退出才能安全释放资源
	// 而工作线程数量动态变化, 用 pthread_detach 避免资源泄漏

	// 等待所有工作线程退出 (live_threads 变为 0)
	while (1)
	{
		pthread_mutex_lock(&mypool->mutex_pool);
		int live = mypool->live_threads;
		pthread_mutex_unlock(&mypool->mutex_pool);

		if (live <= 0)
			break;
		usleep(100000);  // 等待 100ms, 避免忙等
	}

	// 释放动态分配的资源
	free(mypool->workers);              // 工作线程 ID 数组
	queue_destroy(mypool->task_queue);  // 任务队列

	// 销毁同步对象
	pthread_mutex_destroy(&mypool->mutex_pool);
	pthread_mutex_destroy(&mypool->mutex_busy);
	pthread_cond_destroy(&mypool->queue_not_empty);
	pthread_cond_destroy(&mypool->queue_not_full);

	free(mypool);  // 释放线程池结构体本身
}