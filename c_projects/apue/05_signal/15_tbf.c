#include "15_tbf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>  // 多线程互斥锁，保证线程安全

// 令牌桶结构体
struct tbf_st
{
	int token;  // 当前桶中拥有的令牌数
	int cps;    // 每秒生成的令牌数 (速率)
	int burst;  // 桶容量 (令牌上限)
	time_t last_time;  // 上次更新令牌的时间戳 (秒)
};

// 全局桶库: 指针数组, 每个元素指向一个桶或者 NULL(空槽位)
static struct tbf_st *tbf_arr[MAX_TBF];
// 互斥锁, 保护对 tbf_arr 和桶内数据的并发访问
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * 更新指定桶的令牌数 (根据时间差自动补齐)
 * @param tbf 指向要更新的桶结构体
 * 说明 : 必须在持有 mutex 锁的情况下调用
 */
static void update_token(struct tbf_st *tbf)
{
	time_t now = time(NULL);  // 获取当前时间戳
	int delta = now - tbf->last_time;  // 计算距离上次更新的秒数
	if (delta > 0)  // 如果有时间差, 才补充令牌
	{
		int add = delta * tbf->cps;  // 理论上应增加令牌数 = 速率 * 经过秒数
		
		// 如果增加后超过桶容量 (burst - 当前桶中拥有的令牌数)
		if (add > tbf->burst - tbf->token)
			add = tbf->burst - tbf->token;  // 那么 add 限制桶容量的上限值

		tbf->token += add;  // 更新令牌数
		tbf->last_time = now;  // 更新 last_time
	}
}

/**
 * 初始化一个令牌桶
 * @param cps    每秒生成的令牌数 (速率)
 * @param burst  桶的最大容量
 * @return 成功返回桶的描述符 (>=0); 失败返回 -1
 */
int tbf_init(int cps, int burst)
{
	if (cps <= 0 || burst <= 0)
		return -1;

	pthread_mutex_lock(&mutex);  // 加锁, 开始操作全局数组

	// 遍历全局数组, 查找第一个空闲槽位 (值为 NULL)
	for (int tb = 0; tb < MAX_TBF; tb++)
	{
		if (tbf_arr[tb] == NULL)
		{
			// 为新桶开辟内存
			struct tbf_st *new_tbf = malloc(sizeof(struct tbf_st));
			if (new_tbf == NULL)
			{
				// 内存开辟失败, 释放锁并返回错误
				pthread_mutex_unlock(&mutex);
				return -2;
			}

			// 初始化
			new_tbf->token = burst;  // 初始令牌数等于容量 (满桶状态)
			new_tbf->cps = cps;  // 设置速率
			new_tbf->burst = burst;  // 设置桶容量
			new_tbf->last_time = time(NULL);  // 记录当前时间作为起始时间
			tbf_arr[tb] = new_tbf;  // 将桶放进数组
			pthread_mutex_unlock(&mutex);  // 解锁
			return tb;  // 返回数组下标作为桶描述符
		}
	}

	// 遍历完所有槽位都没有空闲位置, 表示已满
	pthread_mutex_unlock(&mutex);
	return -3;
}

/*
* 从指定桶中取走令牌
* @param td  桶的描述符 (由 tbf_init 返回的下标)
* @param n   想要取走的令牌数
* @return 成功返回实际取走的令牌数 (可能少于 n); 失败返回 -1
*/
int tbf_fetch_token(int td, int n)
{
	if (td < 0 || td >= MAX_TBF || n <= 0)
		return -1;

	pthread_mutex_lock(&mutex);  // 加锁保护
	struct tbf_st *tbf = tbf_arr[td];  // 通过下标获取桶指针
	if (tbf == NULL)  // 桶不存在 (已被销毁或者为初始化)
	{
		pthread_mutex_unlock(&mutex);
		return -2;
	}

	// 先根据时间差更新令牌数 (补充新生成的令牌)
	update_token(tbf);

	// 实际能取走的令牌数
	int fetch = (n < tbf->token) ? n : tbf->token;
	tbf->token -= fetch;  // 更新令牌数

	pthread_mutex_unlock(&mutex);
	return fetch;  // 返回实际取走的令牌数
}

/*
* 销毁指定桶
* @param td  桶的描述符
* @return 成功返回 0, 失败返回 -1
*/
int tbf_destroy(int td)
{
	if (td < 0 || td >= MAX_TBF)
		return -1;

	pthread_mutex_lock(&mutex);  // 加锁保护
	if (tbf_arr[td] == NULL)  // 桶不存在
	{
		pthread_mutex_unlock(&mutex);
		return -2;
	}

	free(tbf_arr[td]);  // 释放桶占用的内存
	tbf_arr[td] == NULL;  // 将槽位标记为空闲
	pthread_mutex_unlock(&mutex);  // 解锁
	return 0;  // 销毁成功
}
