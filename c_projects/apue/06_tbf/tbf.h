#ifndef TBF_H
#define TBF_H

/*
	tbf (Token Bucket Filter) 令牌桶过滤器

	设计思路
	● 每个令牌桶结构体 tbf_st 中增加一个 last_time 字段, 用于记录上次更新时间
	● 每次取令牌时, 根据当前时间与 last_time 的差值, 自动计算应补充的令牌数 (cps * 秒数), 并限制不超过 burst
	● 这样无需额外的定时器, 所有逻辑都在取令牌时完成, 避免了信号处理带来的复杂性和线程安全问题
	● 全局使用互斥锁保护指针数组, 确保多线程环境下的安全
*/

/**
 * 初始化一个令牌桶
 * @param cps    每秒生成的令牌数 (速率)
 * @param burst  桶的最大容量
 * @return 成功返回桶的描述符 (>=0); 失败返回 -1
 */
int tbf_init(int cps, int burst);

/*
* 从指定桶中取走令牌
* @param td  桶的描述符 (由 tbf_init 返回的下标)
* @param n   想要取走的令牌数
* @return 成功返回实际取走的令牌数 (可能少于 n); 失败返回 -1
*/
int tbf_fetch_token(int td, int n);

/*
* 销毁指定桶
* @param td  桶的描述符
* @return 成功返回 0, 失败返回 -1
*/
int tbf_destroy(int td);

#endif
