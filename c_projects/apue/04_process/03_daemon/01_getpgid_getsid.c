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
	PID:7320  PPID:7308  PGID:7320  SID:7320
	----------------------------------------
	---------------- child -----------------
	PID:7372  PPID:7320  PGID:7320  SID:7320
	----------------------------------------
*/