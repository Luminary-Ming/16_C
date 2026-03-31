#ifndef ANYTIMER_H
#define ANYTIMER_H

typedef void (*handler_t)(void *);  // 定义回调函数的函数指针

/*
* 功能: 初始化闹钟库
* @param seconds 定时秒数
* @param handler 回调函数
* @param arg 回调函数的参数
* @return 成功返回闹钟库的下标(>=0); 失败返回负数(<0)
*/
int anytimer_init(int seconds, handler_t handler, void *arg);

/*
* 功能: 销毁指定标识的闹钟
* @param timer_id 指定闹钟的标识
* @return 成功返回闹钟库的下标(>=0); 失败返回负数(<0)
*/
int anytimer_destroy(int timer_id);

#endif
