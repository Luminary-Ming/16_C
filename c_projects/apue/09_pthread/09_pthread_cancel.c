#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


static void cleanup_handler1(void *arg)  // 线程终止清理处理函数 1
{
	printf("%s is called!\n", __FUNCTION__);
}

static void cleanup_handler2(void *arg)  // 线程终止清理处理函数 2  (LIFO, 2会先执行)
{
	printf("\n%s is called!\n", __func__);  // __func__ c99 标准
}

// 线程函数, 新线程会执行这个函数
void *thr_job(void *arg)
{
	pthread_cleanup_push(cleanup_handler1, NULL);  // 把终止处理函数 1 压入栈
	pthread_cleanup_push(cleanup_handler2, NULL);  // 把终止处理函数 2 压入栈

	for (int i = 0; i < 10; i++)
	{
		if (i == 5)  // 打印 5 次后退出
			pthread_exit(NULL);  // 会触发清理函数

		write(1, "!", 1);  // 向标准输出 (文件描述符1) 写入一个字符 '!'
		sleep(1);  // 休眠 1s, 让出 CPU 给其他线程
	}

	// 正常退出时, 0 表示不执行清理函数
	pthread_cleanup_pop(0);  // 弹出 handler2, 不执行
	pthread_cleanup_pop(0);  // 弹出 handler1, 不执行

	return NULL;
}

int main(void)
{
	pthread_t tid;  // 定义线程标识符, 用于存储新线程的 ID

	// 创建线程
	// &tid 存储线程 ID 的指针
	// NULL 线程属性, 使用默认属性
	// thr_job 线程要执行的函数
	// NULL 传递给线程函数的参数
	int ret = pthread_create(&tid, NULL, thr_job, NULL);
	if (ret != 0)
	{
		// strerror(ret) 将错误码转换为可读的错误信息
		fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
		exit(1);
	}

	for (int i = 0; i < 3; i++)  // 主线程打印三次后结束
	{
		write(1, "*", 1);  // 向标准输出 (文件描述符1) 写入一个字符 '*'
		sleep(1);  // 休眠 1s
	}

	pthread_join(tid, NULL);  // 等待创建的线程结束

	return 0;
}
/*
	xinming@09_pthread$ ./a.out
	*!!**!!!
	cancel_handler2 is called!
	cancel_handler1 is called!
	xinming@09_pthread$
*/