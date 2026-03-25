#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	printf("PID : %d\n", getpid());  // 获取当前进程的 PID
	printf("PPID : %d\n", getppid());  // 获取父进程的 PID

	sleep(1000);  // 休眠 1000s

	return 0;
}
/*
	新开一个终端, 用命令展示进程关系
	ps -ajx | grep '3354'
	ps -ajx | grep '2671'
*/