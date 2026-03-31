#include "tbf.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define MAX_TBF  1024  // 最大令牌桶数量

// 令牌桶结构体
typedef struct tbf_st
{
	int token;  // 当前桶中拥有的令牌数
	int cps;    // 每秒生成的令牌数 (速率)
	int burst;  // 桶容量 (令牌上限)
}tbf_t;

// 全局桶库: 指针数组, 每个元素指向一个桶或者 NULL(空槽位)
static tbf_t *tbf_libs[MAX_TBF];
static int initd = 0;  // 作为是否启动了令牌桶库的标记; 1启动 0未启动

// SIGALRM 信号处理函数
static void alarm_handler(int none)
{
	alarm(1);  // 1s 闹钟
	for (int i = 0; i < MAX_TBF; i++)
	{
		if (tbf_libs[i] != NULL)
		{
			tbf_libs[i]->token += tbf_libs[i]->cps;  // 给当前令牌桶积攒改令牌桶指定的令牌
			if (tbf_libs[i]->token > tbf_libs[i]->burst)  // 检查当前令牌桶中的令牌数是否超过令牌桶容量
			{
				tbf_libs[i]->token = tbf_libs[i]->burst;  // 把当前令牌桶中的令牌数设置为该令牌桶指定的上限
			}
		}
	}
}

// 启动模块
static void module_load(void)
{
	signal(SIGALRM, alarm_handler);  // 设置 SIGALRM 信号的处理函数
	alarm(1);  // 1s 闹钟
}

// 循环遍历令牌桶库中当前可用最小的令牌桶标记
static int get_tbf_pos(void)
{
	for (int i = 0; i < MAX_TBF; i++)
	{
		if (tbf_libs[i] == NULL)
			return i;
	}
	return -1;  // 没找到返回 -1
}

/**
 * 初始化一个令牌桶
 * @param cps    每秒生成的令牌数 (速率)
 * @param burst  桶的最大容量
 * @return 成功返回桶的描述符 (>=0); 失败返回 -1
 */
int tbf_init(int cps, int burst)
{
	if (cps <= 0 || burst <= 0)  // 检查参数是否合法
		return -1;

	if (initd == 0)  // 令牌桶没有工作
	{
		module_load();  // 启动模块
		initd = 1;  // 切换状态为工作状态
	}

	int pos = get_tbf_pos();  // 获取当前令牌桶中可用的最小桶描述符
	if (pos < 0)
		return -2;

	tbf_libs[pos] = malloc(sizeof(tbf_t));  // 为令牌桶开辟空间
	if (tbf_libs[pos] == NULL)  // 开辟失败
		return -3;

	tbf_libs[pos]->cps = cps;  // 设置客户指定的速率
	tbf_libs[pos]->burst = burst;  // 设置客户指定的桶容量
	tbf_libs[pos]->token = 0;  // 初始化令牌桶中的令牌数为 0

	return pos;  // 返回桶描述符
}

/*
* 从指定桶中取走令牌
* @param td  桶的描述符 (由 tbf_init 返回的下标)
* @param n   想要取走的令牌数
* @return 成功返回实际取走的令牌数 (可能少于 n); 失败返回 -1
*/
int tbf_fetch_token(int td, int n)
{
	if (td < 0 || td >= MAX_TBF || n <= 0)  // 检查参数是否合法
		return -1;

	if (tbf_libs[td] == NULL)  // 检查 td 标记的令牌桶是否存在
		return -2;

	while (tbf_libs[td]->token <= 0)  // 等待 td 令牌桶中积攒令牌
		pause();

	int fetch_token = 0;
	if (tbf_libs[td]->token >= n)  // 检查令牌桶中的令牌是否充足
		fetch_token = n;  // 充足, 取走客户想要的令牌数
	else
		fetch_token = tbf_libs[td]->token;  // 不充足, 有多少取多少

	tbf_libs[td]->token -= fetch_token;  // 扣除取走的令牌

	return fetch_token;  // 返回实际取走的令牌数
}

/*
* 销毁指定桶
* @param td  桶的描述符
* @return 成功返回 0, 失败返回 -1
*/
int tbf_destroy(int td)
{
	if (td < 0 || td >= MAX_TBF)  // 检查参数是否合法
		return -1;

	if (tbf_libs[td] == NULL)  // 检查 td 标记的令牌桶是否存在
		return -2;

	free(tbf_libs[td]);  // 释放 td 令牌桶库
	tbf_libs[td] = NULL;  // 将槽位标记为空闲
	
	return 0;
}

