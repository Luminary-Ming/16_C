#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pool.h"

static void *task1(void *arg)
{
	for (int i = 0; i < 10; i++)
		sleep(1);

	return NULL;
}

int main(void)
{
	pool_t *p = NULL;
	pool_init(&p, MAX_JOB);

	sleep(5);  // 睡 5s

	task_t t;  // 定义任务结构的变量
	for (int i = 0; i < 15; i++)
	{
		t.job = task1;
		t.arg = NULL;
		pool_add_task(p, &t);  // 添加任务
	}

	while (1);
	return 0;
}
/*
	xinming@12_pthread_pool$ ./poll
	XXX  (3 个空闲线程)
	XXX
	XXX
	XXX
	XXX
	OOOXX       (3 忙碌 2 空闲, 总 5 线程)
	OOOOOXX     (5 忙碌 2 空闲, 总 7 线程)
	OOOOOOOXX   (7 忙碌 2 空闲, 总 9 线程)
	[132] _get_free_pos() is failed  (创建第 10 个线程成功, 第 11 个线程失败)
	OOOOOOOOOX  (9 忙碌 1 空闲，总 10 线程)
	OOOOOOOOOO  (10 全忙)
	OOOOOOOOOO
	OOOOOOOOOO
	OOOOOOOOOO
	OOOOOOOOOO
	OOOOOOOOOO
	OOOOOOOOOO
	OOOOOOOOOX
	OOOOOOOOXX
	OOOOOOXXXX
	OOOOOXXXXX
	OOOOOXXXXX
	OOOOOXXXXX
	OOOOOXXXXX
	OOOOOXXXXX
	OOOOOXXXXX
	XXXXXXXXXX
	XXXXXXXX
	XXXXXX
	XXXXXX
	XXXXXX
	XXXXXX
	XXXXXX
	XXXXXX
	XXXXXX
	XXXXXX
*/