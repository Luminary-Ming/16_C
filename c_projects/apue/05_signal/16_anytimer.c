#define _GNU_SOURCE
#include "16_anytimer.h"
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>

struct anytimer_st
{
	int sec;  // 剩余秒数
	handler_t handler;  // 时间到后调用的处理函数
	void *arg;  // 传递给回调函数的参数
};

// 全局闹钟库: 指针数组, 每个元素指向一个闹钟或者为 NULL(空槽位)
static struct anytimer_st *timer_arr[MAX_ANYTIMER];
// 互斥锁, 保护对 timer_arr 的并发访问
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// 标志位, 确保驱动 (定时器) 只初始化一次
static int init_flag = 0;

/**
 * 信号处理函数 (驱动)
 * 每秒被 SIGALRM 信号触发一次, 遍历所有闹钟, 递减剩余秒数, 到期则执行回调并自动销毁闹钟
 * 注意: 信号处理函数中只能使用异步信号安全的函数
 */
static void driver(int none)
{
	// 加锁, 避免与主线程 (初始化、销毁) 冲突
	pthread_mutex_lock(&mutex);
	for (int td = 0; td < MAX_ANYTIMER; td++)
	{
		if (timer_arr[td] != NULL)  // 该槽位有闹钟
		{
			timer_arr[td]->sec--;  // 剩余秒数减 1
			if (timer_arr[td]->sec == 0)  // 时间到
			{
				// 执行用户回调 (用户需确保回调是异步信号安全的)
				timer_arr[td]->handler(timer_arr[td]->arg);
				free(timer_arr[td]);  // 销毁闹钟
				timer_arr[td] = NULL;  // 清空槽位
			}
		}
	}
	pthread_mutex_unlock(&mutex);  // 解锁
}

/**
 * 初始化驱动: 设置 SIGALRM 信号处理函数, 并启动 1 秒周期的定时器
 * 此函数只会被调用一次 (初始化一次)
 */
static void init_driver(void)
{
	struct sigaction sa;   // 信号动作结构体
	struct itimerval itv;  // 定时器结构体

	// 设置信号处理函数
	sa.sa_handler = driver;  // 设置信号处理函数
	sigemptyset(&sa.sa_mask);  // 清空屏蔽字
	sa.sa_flags = SA_RESTART;  // 自动重启被信号中断的系统调用
	sigaction(SIGALRM, &sa, NULL);  // 注册信号处理函数

	// 配置定时器, 每秒触发一次
	itv.it_value.tv_sec = 1;  // 首次触发: 1s 后
	itv.it_value.tv_usec = 0;
	itv.it_interval.tv_sec = 1;  // 重复间隔: 1s
	itv.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &itv, NULL);  // 启动实时定时器
}

/**
 * 初始化闹钟
 * @param sec     定时秒数
 * @param handler 时间到后调用的处理函数
 * @param arg     传递给 handler 的参数
 * @return 成功返回闹钟描述符(>=0); 失败返回 -1
 */
int anytimer_init(int sec, handler_t handler, void *arg)
{
	if (sec < 0 || handler == NULL)
		return -1;

	pthread_mutex_lock(&mutex);  // 加锁开始操作全局数组

	// 如果驱动尚未初始化, 则初始化 (只初始化一次)
	if (!init_flag)
	{
		init_driver();  // 设置信号和定时器
		init_flag = 1;  // 标记已初始化
	}
	
	// 遍历全局数组, 查找第一个空闲槽位 (值为 NULL)
	for (int td = 0; td < MAX_ANYTIMER; td++)
	{
		if (timer_arr[td] == NULL)
		{
			// 为新的闹钟开辟内存
			timer_arr[td] = malloc(sizeof(struct anytimer_st));
			if (timer_arr[td] == NULL)
			{
				// 内存开辟失败, 释放锁并返回错误
				pthread_mutex_unlock(&mutex);
				return -2;
			}

			// 初始化闹钟结构体
			timer_arr[td]->sec = sec;  // 设置剩余秒数
			timer_arr[td]->handler = handler;  // 设置回调函数
			timer_arr[td]->arg = arg;  // 设置回调参数
			pthread_mutex_unlock(&mutex);  // 解锁
			return td;  // 返回数组下标作为描述符
		}
	}

	// 遍历完所有槽位都没找到空闲位置，表示已满
	pthread_mutex_unlock(&mutex);
	return -3;
}

/**
 * 销毁闹钟
 * @param td 闹钟描述符
 * @return 成功返回 0; 失败返回 -1
 */
int anytimer_destroy(int td)
{
	if (td < 0 || td >= MAX_ANYTIMER)
		return -1;

	pthread_mutex_lock(&mutex);  // 加锁保护
	if (timer_arr[td] == NULL)  // 闹钟不存在（重复销毁或未初始化）
	{
		pthread_mutex_unlock(&mutex);
		return -2;
	}

	free(timer_arr[td]);  // 释放闹钟占用的内存
	timer_arr[td] = NULL;  // 将槽位标记为空闲
	pthread_mutex_unlock(&mutex);  // 解锁
	return 0;  // 销毁成功
}

/*
	全局数组 timer_arr 用于管理最多 1024 个闹钟, 描述符即数组下标
	使用 setitimer 和 SIGALRM 实现统一驱动, 每秒触发一次信号
	信号处理函数 driver 中遍历所有闹钟, 递减剩余时间, 到期执行回调并自动销毁
	互斥锁保证多线程环境下对全局数组的安全访问 (信号处理函数中也需加锁, 因为主线程可能并发调用 init/destroy)
	重要: 回调函数 handler 必须在信号处理上下文中是异步信号安全的 (例如只做简单赋值、写管道等), 否则可能导致未定义行为

	 
	------ setitimer 与 闹钟库(anytimer) 做对比 ------

	setitimer 只能同时运行一个定时器 (多个定时器会覆盖前面的定时器)
	
	闹钟库(anytimer) 支持多个独立的闹钟
	统一的时间管理
		闹钟库内部只需要一个系统定时器
		管理所有闹钟的时间递减，大大简化了多定时器的复杂度
	自动内存管理
		闹钟到期后自动销毁，释放资源


	------------ 实际应用场景对比 ------------

	场景1: 游戏开发中的多个定时事件
	// 闹钟库：直观
	anytimer_init(2, spawn_enemy, NULL);      // 2秒后生成敌人
	anytimer_init(5, power_up_appear, NULL);  // 5秒后出现道具
	anytimer_init(1, update_score, &score);   // 1秒后更新分数

	// setitimer：需要自己实现时间调度器


	场景1: 网络服务器需要多个超时检测
	// 使用闹钟库 - 简单清晰
	int timeout1 = anytimer_init(5, handle_timeout, conn1);
	int timeout2 = anytimer_init(10, handle_timeout, conn2);
	int timeout3 = anytimer_init(3, handle_timeout, conn3);

	// 使用 setitimer - 复杂难维护
	// 需要维护一个时间轮或优先队列
	// 每次都要重新计算最早的超时时间
	// 手动管理所有连接的超时状态
*/