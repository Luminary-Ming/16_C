#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main(void)
{
	int i = 114511;  // 存储特定值
	
	pid_t pid = fork();  // 创建子进程
	if (pid < 0)
	{
		perror("fork()");
		exit(1);
	}

	if (pid == 0)  // 子进程
	{
		printf("chlid.i = %p; i = %d\n", &i, i);
		i = 2233;
		printf("chlid.i = %p; i = %d\n", &i, i);
		exit(0);
	}
	else  // 父进程
	{
		wait(NULL);  // 父进程等待子进程结束
		printf("parent.i = %p; i = %d\n", &i, i);
		i = 1919810;
		printf("parent.i = %p; i = %d\n", &i, i);

	}

	return 0;
}
/*
	chlid.i = 0x7fffffffe820; i = 114511
	chlid.i = 0x7fffffffe820; i = 2233
	parent.i = 0x7fffffffe820; i = 114511
	parent.i = 0x7fffffffe820; i = 1919810
*/