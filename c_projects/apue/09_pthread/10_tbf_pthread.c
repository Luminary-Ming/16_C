#include "10_tbf_pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_TBF  1024  // 最大令牌桶数量

// 令牌桶结构体
typedef struct tbf_st
{
	int token;  // 当前桶中拥有的令牌数
	int cps;    // 每秒生成的令牌数 (速率)
	int burst;  // 桶容量 (令牌上限)

	pthread_mutex_t mutex;  // 表示当前令牌桶的互斥锁
	pthread_cond_t cond;  // 表示当前令牌桶的条件变量
}tbf_t;

// 全局桶库: 指针数组, 每个元素指向一个桶或者 NULL(空槽位)
static tbf_t *tbf_libs[MAX_TBF];
static int initd = 0;  // 作为是否启动了令牌桶库的标记; 1启动 0未启动

static pthread_mutex_t mutex_job = PTHREAD_MUTEX_INITIALIZER;  // 初始化令牌桶的互斥量
static pthread_t jobtid;  // 定义全局的线程标识符

// 线程工作函数
static void *thr_job(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&mutex_job);  // 抢令牌桶库的锁

		for (int i = 0; i < MAX_TBF; i++)  // 循环变量每个令牌桶
		{
			if (tbf_libs[i] != NULL)  // 检查改位置的令牌桶是否在用
			{
				pthread_mutex_lock(&tbf_libs[i]->mutex);  // 抢指定令牌桶中的锁
				
				tbf_libs[i]->token += tbf_libs[i]->cps;  // 积攒令牌
				
				if (tbf_libs[i]->token > tbf_libs[i]->burst)  // 检查令牌是否到达上限
					tbf_libs[i]->token = tbf_libs[i]->burst;

				pthread_mutex_unlock(&tbf_libs[i]->mutex);  // 释放指定令牌桶中的锁

				pthread_cond_broadcast(&tbf_libs[i]->cond);  // 让指定令牌桶发送条件变量变化的通知
			}
		}

		pthread_mutex_lock(&mutex_job);  // 释放令牌桶库的锁
		sleep(1);  // 休眠1s, 也是为了设置取消点函数
	}

	pthread_exit(NULL);  // 终止线程
}

// 卸载模块
static void module_unload(void)
{
	pthread_cancel(jobtid);  // 给 jobtid 线程发送取消请求

	pthread_join(jobtid, NULL);  // 等待 jobtid 线程结束

	for (int i = 0; i < MAX_TBF; i++)
	{
		if (tbf_libs[i] != NULL)  // 检查当前令牌是否可用
			tbf_destroy(i);  // 释放令牌桶
	}
}

// 加载模块
static void module_load(void)  
{
	// 创建线程
	int ret = pthread_create(&jobtid, NULL, thr_job, NULL);
	if (ret != 0)  // 创建失败
	{
		fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
		exit(1);
	}

	atexit(module_unload);  // 注册进程终止处理函数
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

	pthread_mutex_init(&tbf_libs[pos]->mutex, NULL);  // 初始化化令牌桶中的互斥锁
	pthread_cond_init(&tbf_libs[pos]->cond, NULL);  // 初始化令牌桶中的条件变量

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

	pthread_mutex_lock(&tbf_libs[td]->mutex);  // 抢指定令牌桶的锁

	while (tbf_libs[td]->token <= 0)  // 等待 td 令牌桶中积攒令牌
		pthread_cond_wait(&tbf_libs[td]->cond, &tbf_libs[td]->mutex);  // 等待指定令牌桶的条件变量变化

	// 不足: 如果当前桶中的令牌数少于客户想取走的令牌数, 那就令牌桶中有多少取多少
	// 充足: 如果当前桶中的令牌数大于客户想取走的令牌数, 那就客户想取多少取多少
	int fetch_token = tbf_libs[td]->token <= n ? tbf_libs[td]->token : n;

	tbf_libs[td]->token -= fetch_token;  // 扣除取走的令牌

	pthread_mutex_unlock(&tbf_libs[td]->mutex);  // 释放指定令牌桶的锁

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

	pthread_mutex_destroy(&tbf_libs[td]->mutex);  // 销毁互斥锁
	pthread_cond_destroy(&tbf_libs[td]->cond);  // 销毁条件变量

	free(tbf_libs[td]);  // 释放 td 令牌桶库
	tbf_libs[td] = NULL;  // 将槽位标记为空闲

	return 0;
}


/*
* 返还令牌
* @param td 桶的描述符
* @param ntoken 要返还的令牌数量
* @return 成功返回 0, 失败返回负数
*/
int tbf_return_token(int td, int ntoken)
{
	if (td < 0 || td >= MAX_TBF || ntoken <= 0)  // 检查参数是否合法
		return -1;

	if (tbf_libs[td] == NULL)  // 判断 td 标识的令牌桶是否不存在
		return -2;

	pthread_mutex_lock(&tbf_libs[td]->mutex);  // 抢指定令牌桶的锁

	tbf_libs[td]->token += ntoken;  // 返回令牌

	if (tbf_libs[td]->token > tbf_libs[td]->burst)  // 检查令牌数是否超过上限
		tbf_libs[td]->token = tbf_libs[td]->burst;

	pthread_mutex_unlock(&tbf_libs[td]->mutex);  // 释放指定令牌桶的锁

	pthread_cond_broadcast(&tbf_libs[td]->cond);  // 给指定令牌桶中发送条件变量变化的通知\

	return 0;
}

/*
* 释放所有令牌桶
*/
void tbf_destroy_all(void)
{
	pthread_mutex_lock(&mutex_job);  // 抢令牌桶库的锁

	for (int i = 0; i < MAX_TBF; i++)  // 循环销毁令牌桶
	{
		if (tbf_libs[i] != NULL)
			tbf_destroy(i);
	}

	pthread_mutex_unlock(&mutex_job);  // 释放令牌桶库的锁
}
