#ifndef ANYTIMER_H
#define ANYTIMER_H

/*
	闹钟库 (anytimer)

	闹钟结构体包含剩余秒数、回调函数指针、参数
	全局使用 setitimer 产生每秒一次的 SIGALRM 信号
	信号处理函数中遍历所有闹钟, 将剩余秒数减1, 若归零则调用回调函数并自动销毁该闹钟
	使用互斥锁保护全局数组, 防止信号处理与主线程冲突
	(注意信号处理函数中必须使用异步信号安全的函数, 这里我们只做简单操作, 回调由用户提供, 用户需确保回调是异步信号安全的)
*/

#define MAX_ANYTIMER 1024  // 最大支持 1024 个闹钟

typedef void (*handler_t)(void *);

/**
 * 初始化闹钟
 * @param sec     定时秒数
 * @param handler 时间到后调用的处理函数
 * @param arg     传递给 handler 的参数
 * @return 成功返回闹钟描述符(>=0); 失败返回 -1
 */
int anytimer_init(int sec, handler_t handler, void *arg);

/**
 * 销毁闹钟
 * @param td 闹钟描述符
 * @return 成功返回 0; 失败返回 -1
 */
int anytimer_destroy(int td);

#endif
