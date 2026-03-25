#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(void)
{
	printf("---------------- parent ----------------\n");
	printf("PID:%d  PPID:%d  PGID:%d  SID:%d",
		   getpid(), getppid(), getpgid(0), getsid(0));  // 打印父进程信息
	printf("\n----------------------------------------\n");

	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork()");
		exit(1);
	}

	if (pid == 0)  // 子进程
	{
		if (setpgid(getpid(), getpid()) != 0)  // 为子进程设置进程组
			perror("setpgid()");
		printf("---------------- child -----------------\n");
		printf("PID:%d  PPID:%d  PGID:%d  SID:%d",
			   getpid(), getppid(), getpgid(0), getsid(0));  // 打印子进程信息
		printf("\n----------------------------------------\n");
		exit(0);  // 终止子进程, 返回状态 0
	}

	wait(NULL);  // 父进程等待子进程结束

	return 0;
}
/*
	---------------- parent ----------------
	PID:15609  PPID:15597  PGID:15609  SID:15609
	----------------------------------------
	---------------- child -----------------
	PID:15662  PPID:15609  PGID:15662  SID:15609
	----------------------------------------
*/