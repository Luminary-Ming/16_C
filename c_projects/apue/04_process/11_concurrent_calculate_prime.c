/*
	筛选出100 ~ 300之间所有的质数,每次判断之前延时 1s (需要并发实现)

	创建 201 子进程检测质数, 所有子进程几乎同时启动, 它们的 sleep(1) 会几乎同时结束
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MIN 100
#define MAX 300
#define NUM (MAX - MIN + 1)  // 子进程个数

int is_prime(int num);

int main(void)
{
	for (int i = MIN; i <= MAX; i++)
	{
		pid_t pid = fork();  // 创建子进程
		if (pid < 0)
		{
			perror("fork()");
			exit(1);
		}
		if (pid == 0)  // 子进程
		{
			sleep(1);  // 每次判断前延时1秒

			if (is_prime(i))
				printf("%d Is A Prime Number\n", i);

			exit(0);  // 终止子进程, 并且返回状态 0
		}	
	}	

	for (int i = 0; i < NUM; i++)
		wait(NULL);

	return 0;
}

int is_prime(int num)
{
	if (num == 0 || num == 1) return 0;
	if (num == 2) return 1;
	if (num % 2 == 0) return 0;

	for (int i = 3; i * i <= num; i += 2)
	{
		if (num % i == 0)
			return 0;
	}

	return 1;
}
