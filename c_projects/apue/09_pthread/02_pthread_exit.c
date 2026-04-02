#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// 线程函数, 新线程会执行这个函数
void *thr_job(void *arg)
{
	for (int i = 0; i < 10; i++)
	{
		if (i == 5)
		{
			// 三种退出都可以
			//pthread_exit((void *)0);
			//pthread_exit(NULL);
			return NULL;
		}

		write(1, "!", 1);  // 向标准输出 (文件描述符1) 写入一个字符 '!'
		sleep(1);  // 休眠 1s, 让出 CPU 给其他线程
	}

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

	while (1)  // 主线程的无限循环
	{
		write(1, "*", 1);  // 向标准输出 (文件描述符1) 写入一个字符 '*'
		sleep(1);  // 休眠 1s
	}
}

/*
	xinming@09_pthread$ ./a.out
	*!!*!**!*!*********^C
	xinming@09_pthread$

	线程打印五次 '!' 就 "return NULL" 退出了, 后面就一直打印 '*' 了
*/
