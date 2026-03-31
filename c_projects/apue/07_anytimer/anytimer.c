#include "anytimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define ANYTIMER_MAX 1024  // 最大支持 1024 个闹钟

typedef struct
{
	int remain;  // 剩余的秒数
	handler_t callback;  // 闹钟时间到后执行的回调函数
	void *arg;  // 回调函数的参数
	int is_used;  // 标记当前闹钟是否被使用(1=使用中 0=未被使用)
}anytimer_st;

// 全局闹钟库数组
static anytimer_st timer_libs[ANYTIMER_MAX];
static int lib_inited = 0;  // 标记闹钟库是否初始化(0=未被初始化 1=已初始化)

// SIGALRM 信号处理函数: 遍历所有闹钟, 更新剩余秒数
static void alarm_handler(int none)
{
	alarm(1);  // 1s 闹钟

	for (int i = 0; i < ANYTIMER_MAX; i++)
	{
		if (timer_libs[i].is_used == 1)  // 检查当前闹钟是否正在被使用
		{
			timer_libs[i].remain--;  // 更新剩余秒数
			if (timer_libs[i].remain == 0)  // 判断该闹钟是否到了定时时间
			{
				timer_libs[i].callback(timer_libs[i].arg);  // 执行回调函数
				timer_libs[i].is_used = 0;  // 执行完回调函数之后, 标记闹钟为空闲状态
			}
		}
	}
}

// 初始化闹钟库
static void lib_init(void)
{
	for (int i = 0; i < ANYTIMER_MAX; i++)  // 循环遍历初始化所有闹钟为空闲
	{
		timer_libs[i].remain = 0;
		timer_libs[i].callback = NULL;
		timer_libs[i].arg = NULL;
		timer_libs[i].is_used = 0;
	}
	signal(SIGALRM, alarm_handler);  // 设置 SIGALRM 信号的处理函数
	alarm(1);  // 1s 闹钟
	lib_inited = 1;  // 切换闹钟为已初始化状态
}

// 获取当前闹钟库中可用最小的闹钟标识
static int get_anytimer_pos(void)
{
	for (int i = 0; i < ANYTIMER_MAX; i++)
		if (timer_libs[i].is_used == 0)  // 检查当前闹钟库下标位置的闹钟是否空闲可用
			return i;
	return -1;  // 没有空闲的闹钟返回 -1
}

/*
* 功能: 初始化闹钟库
* @param seconds 定时秒数
* @param handler 回调函数
* @param arg 回调函数的参数
* @return 成功返回闹钟库的下标(>=0); 失败返回负数(<0)
*/
int anytimer_init(int seconds, handler_t handler, void *arg)
{
	if (seconds <= 0 || handler == NULL)  // 判断参数是否合法
		return -1;

	if (lib_inited == 0)  // 检查是否是第一次使用闹钟库
		lib_init();  // 初始化闹钟库

	int pos = get_anytimer_pos();  // 获取当前闹钟库中可用最小的闹钟标识
	if (pos < 0)
		return -2;

	timer_libs[pos].remain = seconds;  // 设置客户指定的秒数
	timer_libs[pos].callback = handler;  // 设置客户指定的回调函数
	timer_libs[pos].arg = arg;  // 设置客户指定的回调函数的参数
	timer_libs[pos].is_used = 1;  // 设置当前闹钟的状态为使用状态

	return pos;
}

/*
* 功能: 销毁指定标识的闹钟
* @param timer_id 指定闹钟的标识
* @return 成功返回闹钟库的下标(>=0); 失败返回负数(<0)
*/
int anytimer_destroy(int timer_id)
{
	if (timer_id < 0 || timer_id >= ANYTIMER_MAX)  // 检查参数是否合法
		return -1;

	if (timer_libs[timer_id].is_used == 0)  // 检查当前闹钟是否被使用
		return -2;

	// 客户指定的闹钟清空
	timer_libs[timer_id].remain = 0;
	timer_libs[timer_id].callback = NULL;
	timer_libs[timer_id].arg = NULL;
	timer_libs[timer_id].is_used = 0;

	return 0;
}
