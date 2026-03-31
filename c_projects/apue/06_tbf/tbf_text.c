#include "tbf.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define TBFMAX 1024

struct tbf_st
{
	int token;  // 表示当前令牌桶的令牌数
	int cps;  // 表示当前令牌桶的速率
	int burst; // 当前令牌桶的容量
}tbf_t;  // 定义了令牌桶的数据结构

static tbf_t *tbf_libs[TBFMAX];  // 令牌桶库的定义
static int initd;  // 作为是否启动了令牌桶的标记
//(0表示没有工作 1表示开工作)

static int get_tbf_pos(void)  // 循环遍历令牌桶库中当前可用最小令牌桶的标记
{
	int i = 0;
	for (i = 0; i < TBFMAX; i++)
		if (tbf_libs[i] == NULL)  // 判断当前位置是否可用
			return i;  // 返回当前可用位置下标
	return -1;
}

static void alarm_handler(int none)  // SIGALRM信号行为
{
	int i = 0;
	alarm(1);  // 设置1s的闹钟
	for (i = 0; i < TBFMAX; i++)  // 循环遍历
	{
		if (ftb_libs[i] != NULL)  // 判断当前令牌桶是否正在使用
		{
			tbf_libs[i]->token += tbf_lib[i]->cps;  // 给当前令牌桶积攒该令牌桶指定的令牌
			if (tbf_libs[i] > tbf_libs[i]->burst)  // 判断当前令牌桶中令牌数是否超过该令牌桶指定的上限
				tbf_libs[i]->token = tbf_libs[i]->burst;  // 把当前令牌桶中令牌数设置为该令牌桶的上限
		}
	}

}

static void module_load(void)  // 启动模块
{
	signal(SIGALRM, alarm_handler);  // 给SIGALEM信号设置行为
	alarm(1);  // 设置1s的闹钟
}

int tbf_init(int cps, int burst)
{
	int pos = 0;  // pos变量存储到当前可用最小令牌桶库的标记

	if (cps <= 0 || burst <= 0)  // 判断形参是否没有意义
		return -1;
	if (!initd)  // 判断令牌桶是否没有工作
	{
		module_load();  // 启动模块
		initd = 1;  // 切换工作状态
	}
	pos = get_tbf_pos();  // 获取当前令牌桶中可用最小令牌桶库的标记
	if (pos < 0)  // 判断是否没有找到合适位置
		return -2;

	tbf_libs[pos] = malloc(sizeof(tbf_t));  // 为令牌桶开辟空间
	if (tbf_libs[pos] == NULL)  // 判断开辟空间是否失败
		return -3;
	tbf_libs[pos]->cps = cps;  // 存储用户指定的速率
	tbf_libs[pos]->burst = burst;  // 存储用户指定的上限
	tbf_libs[pos]->token = 0;  // 初始化令牌桶中令牌数为0

	return pos;  // 返回当前初始化成功的令牌桶的下标
}

int tbf_fetch_token(int td, int n)
{
	int fetch_token = 0;  // 存储取走的令牌数

	if (td < 0 || td >= TBFMAX || n <= 0)  // 判断参数是否没有意义
		return -1;
	if (tbf_libs(td) == NULL)  // 判断td标记的令牌桶是否不存在
		return -2;
	while (tbf_libs[td]->token <= 0)  // 等待td令牌桶内积攒令牌
		pause();  // 等通知
	if (tbf_libs[td]->token >= 0)  // 判断令牌桶内的令牌是否充足
		fetch_token = n;  // 取走客户指定令牌个数
	else
		fetch_token = tbf_libs[td]->token;  // 有多少取多少
	tbf_libs[td]->token -= fetch_token;  // 更新tb令牌桶内的令牌数

	return fetch_token;  // 返回成功取走令牌数
}

int tbf_destroy(int td)
{

	if (td < 0 || td >= TBFMAX || n <= 0)  // 判断参数是否没有意义
		return -1;
	if (tbf_libs(td) == NULL)  // 判断td标记的令牌桶是否不存在
		return -2;

	free(tbf_libs[td]);  // 释放td令牌桶
	tbf_libs[td] = NULL;  // 避免出现野指针

	return 0;  // 释放成功，返回0
}