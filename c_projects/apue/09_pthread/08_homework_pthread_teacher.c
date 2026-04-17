/*
	多线程顺序输出字符
	使用 5 个线程分别循环输出'a'、'b'、'c'、'd'、'e'，
	并通过互斥锁和条件变量确保输出顺序严格为 "abcdeabcde..."
	程序运行5秒后自动退出
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define THREAD_NUM 5  // 线程数量

/*
job 变量的状态机 :
	-1 空闲, 没有发放任务
	 0 表示打印 'a' 的线程可以工作
	 1 表示打印 'b' 的线程可以工作
	 2 表示打印 'c' 的线程可以工作
	 3 表示打印 'd' 的线程可以工作
	 4 表示打印 'e' 的线程可以工作
*/
static int job = -1;  // 初始化临界区资源

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 初始化互斥锁
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  // 初始化条件变量

static int running = 1;  // 全局运行标志

// 线程工作函数
static void *thr_job(void *arg)
{
	int n = *(int *)arg;  // 线程编号

	while (running)
	{
		pthread_mutex_lock(&mutex);  // 加锁

		while (n != job)  // 只有编号匹配的线程才能工作
			/*
				pthread_cond_wait 会释放锁, 允许其他线程进入临界区
				当条件不满足时, 线程自愿放弃 CPU, 进入睡眠状态
				被唤醒后, 它会重新获得锁, 然后再次检查条件
			*/
			pthread_cond_wait(&cond, &mutex);  // 等待条件变量变化的通知
			/*
				当线程进入 while (n != job) 循环时
					1.它会释放互斥锁, 让其他线程得以工作
					2.它会阻塞自己, 不占用 CPU 时间
					3.被唤醒后会重新获得锁并再次检查条件
					4.只有编号匹配的线程才能跳出循环执行打印
					5.其他不匹配的线程会再次进入等待, 形成严格的顺序执行
			*/

		putchar('a' + n);  // 根据编号打印相应的字符
		fflush(stdout);  // putchar 有缓冲区, 刷新一下

		job = -1;  // 修改临界区的状态, 表示任务完成

		pthread_cond_broadcast(&cond);  // 发送条件变量变化的通知 (通知主线程可以发放新任务)
		pthread_mutex_unlock(&mutex);  // 解锁
	}

	return NULL;
 }

// 信号处理函数
void alarm_handler(int none)
{
	running = 0;
}

int main(void)
{
	signal(SIGALRM, alarm_handler);  // 注册信号处理函数
	alarm(5);  // 设置 5s 的闹钟

	// 循环创建线程
	pthread_t tids[THREAD_NUM];  // 存储线程 ID
	int thread_num[THREAD_NUM];  // 存储线程编号 (0 ~ 4)
	for (int i = 0; i < THREAD_NUM; i++)
	{
		thread_num[i] = i;
		int ret = pthread_create(&tids[i], NULL, thr_job, &thread_num[i]);
		if (ret != 0)  // 创建失败
		{
			fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
			exit(1);
		}
	}

	// 在 5s 时间内疯狂发送任务
	for (int i = 0; ; i = (i + 1) % THREAD_NUM)
	{
		pthread_mutex_lock(&mutex);  // 加锁

		while (job != -1)  // 等待上一个任务完成
			pthread_cond_wait(&cond, &mutex);  // 等待条件变量变化的通知

		job = i;  // 发放新任务
		pthread_cond_broadcast(&cond);  // 发送条件变量变化的通知
		pthread_mutex_unlock(&mutex);  // 解锁
	}

	// 等待所有线程结束
	for (int i = 0; i < THREAD_NUM; i++)
		pthread_join(tids[i], NULL);

	// 释放资源
	pthread_mutex_destroy(&mutex);  // 销毁互斥锁
	pthread_cond_destroy(&cond);  // 销毁条件变量

	return 0;
}