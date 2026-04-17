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

// 全局同步变量
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 初始化互斥锁
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  // 初始化条件变量
int turn = 0;  // 当前应该输出的线程编号 (0 ~ 4)
int running = 1;  // 程序运行状态, 1=运行中; 0=结束

// 线程工作函数的参数结构体
struct thread_arg
{
	int tid;  // 线程编号
	char ch;  // 对应输出的英文字符
};

// 线程工作函数
void *thread_job(void *arg)
{
	struct thread_arg *thr_arg = (struct thread_arg *)arg;
	int tid = thr_arg->tid;  // 获取线程编号
	char ch = thr_arg->ch;  // 获取对应要输出的英文字符
	free(thr_arg);  // 释放内存

	while (1)  // 死循环, 直到收到结束信号并主动退出
	{
		pthread_mutex_lock(&mutex);  // 加锁

		// 等待条件: 当前线程的 ID 不等于 turn (未轮到自己输出), 并且程序仍在运行 (running == 1)
		while (tid != turn && running == 1)
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

		// 如果程序已结束, 退出循环
		if (running == 0)
		{
			pthread_mutex_unlock(&mutex);  // 解锁
			break;
		}

		write(STDOUT_FILENO, &ch, 1);  // 往 stdout 中写入英文字符

		turn = (turn + 1) % THREAD_NUM;  // 更新下一个要输出字符的线程 ID

		pthread_cond_broadcast(&cond);  // 唤醒所有等待的线程 (让下一个线程继续)
		pthread_mutex_unlock(&mutex);  // 解锁
	}

	return NULL;
}

// SIGALRM 信号处理函数
static void alarm_handler(int none)
{
	// 什么也不需要做
}


int main(void)
{
	// 设置信号处理函数
	struct sigaction sa;   // 信号动作结构体
	sa.sa_handler = alarm_handler;  // 设置信号处理函数
	sigemptyset(&sa.sa_mask);  // 清空屏蔽字
	sa.sa_flags = SA_RESTART;  // 自动重启被信号中断的系统调用
	sigaction(SIGALRM, &sa, NULL);  // 注册信号处理函数

	// 配置定时器
	struct itimerval itv;  // 定时器结构体
	itv.it_value.tv_sec = 5;  // 首次触发时间 5s
	itv.it_value.tv_usec = 0;
	itv.it_interval.tv_sec = 0;  // 重复间隔时间 0s
	itv.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &itv, NULL);  // 启动实时定时器

	// 循环创建线程
	pthread_t tids[THREAD_NUM];  // 线程 ID 数组
	for (int i = 0; i < THREAD_NUM; i++)
	{
		// 创建参数
		struct thread_arg *arg = malloc(sizeof(struct thread_arg));
		if (arg == NULL)  // 开辟失败
		{
			perror("malloc()");
			exit(1);
		}
		arg->tid = i;        // 0,   1,   2,   3,   4
		arg->ch = 'a' + i;  // 'a', 'b', 'c', 'd', 'e'

		// 创建线程
		int ret = pthread_create(&tids[i], NULL, thread_job, arg);
		if (ret != 0)  // 创建失败
		{
			fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
			exit(2);
		}
	}

	// 主线程挂起, 等待 SIGALRM 信号
	pause();

	// 信号到达, 通知所有线程终止退出
	pthread_mutex_lock(&mutex);  // 加锁
	running = 0;  // 结束
	pthread_cond_broadcast(&cond);  // 广播通知所有线程结束
	pthread_mutex_unlock(&mutex);  // 解锁

	// 等待所有线程结束, 并回收资源
	for (int i = 0; i < THREAD_NUM; i++)
		pthread_join(tids[i], NULL);

	// 清理资源
	pthread_mutex_destroy(&mutex);  // 销毁互斥锁
	pthread_cond_destroy(&cond);  // 销毁条件变量

	return 0;  // 程序正常结束
}