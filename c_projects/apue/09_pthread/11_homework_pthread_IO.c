/*
	main 线程将 0 写入 "/tmp/out" 文件,
	并发 20 个工作线程,
	所有的工作线程都是将 "/tmp/out" 文件中的数据读出来 +1 再写回去,
	要求文件中的最终数值一定是 20

	思路 :
	让线程 1 写 1
	让线程 2 写 2
	让线程 3 写 3
	...
	让线程 20 写 20
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// 宏定义
#define THREAD_NUM 20  // 线程数量
#define FILE_PATH "/tmp/out"  // 文件路径
#define BUF_SIZE 3  // 缓冲区大小

// 全局变量
int fd = -1;  // 文件描述符
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 初始化互斥锁
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  // 初始化条件变量
int turn = 1;  // 当前应该工作的线程编号 (1 ~ 20)
int running = 1;  // 程序运行状态, 1=运行中; 0=结束

// 线程工作函数
static void *thread_job(void *arg)
{
	int thread_num = *(int *)arg;  // 获取线程编号

	while (1)
	{
		pthread_mutex_lock(&mutex);  // 加锁

		// 等待轮到自己, 如果当前线程不是应该工作的线程则等待 (不是你该干的活, 你就等着吧)
		while (thread_num != turn && running == 1)
			pthread_cond_wait(&cond, &mutex);  // 等待条件变量变化的通知

		// 程序已结束, 退出线程
		if (running == 0)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}

		// 读数据 (每次回到开头读)
		char buf[BUF_SIZE] = { 0 };  // 定义缓冲区
		lseek(fd, 0, SEEK_SET);  // 定位到开头 
		ssize_t read_size = read(fd, buf, BUF_SIZE - 1);  // 读取 fd 文件, 只读前 2 字节, 读出的内容放到 buf 中
		if (read_size == -1)  // 读到错误
		{
			perror("read()");
			pthread_mutex_unlock(&mutex);  // 解锁
			pthread_exit(NULL);
		}
		buf[read_size] = '\0';  // 确保尾零结尾

		int num = atoi(buf);  // 转为整型数字

		if (num == 20)  // 检查任务是否已完成
		{
			running = 0;  // 标记程序结束
			pthread_cond_broadcast(&cond);  // 唤醒通知所有线程程序已结束
			pthread_mutex_unlock(&mutex);  // 解锁
			break;
		}

		num++;  // 数字 +1

		// 写数据 (每次回到开头覆盖写)
		lseek(fd, 0, SEEK_SET);  // 定位到开头
		int len = snprintf(buf, BUF_SIZE, "%d", num);  // 格式化字符串
		write(fd, buf, len);
		fsync(fd);  // 确保数据写入磁盘

		// 打印输出信息
		printf("[thread %2d]: write %d\n", thread_num, num);

		turn = turn % THREAD_NUM + 1;  // 轮到下一个线程, 始终在 1 ~ 20 之间循环

		pthread_cond_broadcast(&cond);  // 唤醒所有等待的线程 (让下一个线程继续)
		pthread_mutex_unlock(&mutex);  // 解锁
	}

	return NULL;
}

int main(void)
{
	// 打开文件, O_TRUNC 截断文件, 清空数据 (如果 /tmp/out 文件已存在且包含旧数据, 不加 O_TRUNC 不会清空)
	fd = open(FILE_PATH, O_CREAT | O_RDWR | O_TRUNC, 0644);  // 创建文件, 以读写的方式打开文件
	if (fd == -1)
	{
		perror("open()");
		exit(1);
	}

	// 初始主线程向 fd 文件中写入一个字符 '0'
	write(fd, "0", 1);

	// 循环创建线程
	pthread_t tids[THREAD_NUM];  // 存储线程 ID
	int thread_num[THREAD_NUM];  // 存储线程编号
	for (int i = 0; i < THREAD_NUM; i++)
	{
		thread_num[i] = i + 1;  // 线程编号, 从 1 开始

		int ret = pthread_create(&tids[i], NULL, thread_job, &thread_num[i]);
		if (ret != 0)  // 创建失败
		{
			fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
			exit(2);
		}
	}

	// 等待所有线程结束
	for (int i = 0; i < THREAD_NUM; i++)
		pthread_join(tids[i], NULL);

	// 释放资源
	close(fd);  // 关闭文件
	pthread_mutex_destroy(&mutex);  // 销毁互斥锁
	pthread_cond_destroy(&cond);  // 销毁条件变量

	return 0;
}