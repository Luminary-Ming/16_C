/*
	多线程质数检测程序
	使用互斥锁和轮询机制来协调主线程（任务分配者）和工作线程（任务执行者）之间的同步

	轮询法 : 存在轮询消耗 CPU 资源的问题
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>

#define MIN 100  // 待检测数字范围的最小值
#define MAX 300  // 待检测数字范围的最大值
#define THREAD_NUM 4  // 工作线程数量 (通常设为 CPU 核心数)

/*
job 变量的状态机 :
	 0  空闲, 无待处理任务, 可以分配新任务
	>0  有任务待处理, 工作线程尚未取走
	-1  所有任务分配完毕, 工作线程应退出
*/
static int job = 0;  // 临界资源, 需要互斥访问

/*
	job 变量是静态的全局变量, 所有线程都回来访问 / 修改 job, 所以就是临界区
	容易出现竞态, 为了避免出现竞态, 需要给临界区加互斥锁 (互斥量)
*/
// ========== [1] 初始化互斥量 (要么调用 pthread_mutex_init(3) 或者使用宏) ==========
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 初始化互斥量 (互斥锁)

// 判断质数
static int is_prime(int n)
{
	if (n <= 1) return 0;
	if (n == 2) return 1;  // 2 是最小的质数
	if (n % 2 == 0) return 0;  // // 偶数都不是质数

	for (int i = 3; i * i <= n; i += 2)  // 只检查奇数因子，且只检查到平方根即可
	{
		if (n % i == 0)
			return 0;  // 能被整除, 不是质数
	}

	return 1;  // 是质数
}


// 工作线程函数
// 循环抢占任务，直到遇到 job == -1 时退出
void *thr_job(void *arg)
{
/*
	循环抢任务
	[1] 遇到 job == -1 说明所有任务分配完毕 -> 终止线程
	[2] 遇到 job == 0  说明无待处理任务, 可以分配新任务 -> 等着抢任务
	[3] 遇到 job > 0   有任务待处理 -> 处理任务
*/

	int n = 0;  // 保存从临界区取出的任务值

	while (1)
	{
		pthread_mutex_lock(&mutex);  // 加锁

		if (job == -1)  // 所有任务已分配完毕
		{
			pthread_mutex_unlock(&mutex);  // 解锁
			pthread_exit(0);  // 终止线程
		}

		if (job == 0)  // 当前无待处理任务
		{
			pthread_mutex_unlock(&mutex);  // 解锁
			continue;  // 继续轮询等待新任务
		}

		// 有任务可取, 取出任务并将 job 重置为 0
		n = job;
		job = 0;

		pthread_mutex_unlock(&mutex);  // 解锁

		// 执行任务 (耗时操作放在临界区之外)
		if (is_prime(n))
			printf("%d is a prime !\n", n);

		sleep(1);  // 模拟工作线程的复杂任务, 查看 CPU 占用资源率
	}

	return NULL;
}


int main(void)
{
	/*
		[1] 初始化互斥量
		[2] 创建所有的工作线程 (pthread_create(3))
		[3] 发放任务
		[4] 告诉工作线程们, 所有任务发放完毕
		[5] main() 线程等待工作线程们结束
		[6] 销毁互斥量
	*/

	// ========== [2] 创建所有的工作线程 (pthread_create(3)) ==========
	pthread_t tid[THREAD_NUM];  // 存储 THREAD_NUM 个线程的标识

	// 创建 CHILD_NUM 个线程
	for (int i = 0; i < THREAD_NUM; i++)
	{
		int ret = pthread_create(tid + i, NULL, thr_job, NULL);
		if (ret != 0)
		{
			// strerror(ret) 将错误码转换为可读的错误信息
			fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
			exit(1);
		}
	}


	// ========== [3] 发放任务, 依次将 MIN 到 MAX 的数字分配给工作线程 ==========
	for (int i = MIN; i <= MAX; i++)
	{
		pthread_mutex_lock(&mutex);  // 加锁

		while (job > 0)  // 轮询等待 job == 0 (就轮询等待其他线程取走任务)
		{
			// 主线程在分配新任务前, 会反复加锁/解锁互斥量检查 job 状态, 直到工作线程取走上一个任务
			pthread_mutex_unlock(&mutex);  // 解锁
			pthread_mutex_lock(&mutex);  // 加锁
		}

		job = i;  // main() 线程分配新任务
		pthread_mutex_unlock(&mutex);  // 解锁
	}


	// ========== [4] 通知所有工作线程任务分配完毕 (也就是 job = -1) ==========
	pthread_mutex_lock(&mutex);  // 加锁

	// 等待最后一个任务被取走
	while (job > 0)  // 轮询等待 job == 0 (就轮询等待其他线程取走任务)
	{
		// 主线程在分配新任务前, 会反复加锁/解锁互斥量检查 job 状态, 直到工作线程取走的最后任务
		pthread_mutex_unlock(&mutex);  // 解锁
		pthread_mutex_lock(&mutex);  // 加锁
	}

	job = -1;  // main() 线程改变临界区状态为分配完毕
	pthread_mutex_unlock(&mutex);  // 解锁


	// ========== [5] main() 线程等待工作线程们结束 ==========
	for (int i = 0; i < THREAD_NUM; i++)
		pthread_join(tid[i], NULL);

	// ========== [6] 销毁互斥量 (互斥锁) ==========
	pthread_mutex_destroy(&mutex);

	return 0;
}

