#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	int status;

	pid_t pid = fork();  // 创建子进程
	if (pid < 0)  // 检查创建子进程是否失败
	{
		perror("fork()");
		exit(1);
	}

	if (pid == 0)  // 子进程
	{
		printf("======== 子进程 (PID:%d) 开始执行 ========\n", getpid());

		sleep(10);  // 休眠 10s, 模拟子进程进行任务的处理时间

		printf("======== 子进程 (PID:%d) 进行完毕,准备退出 ========\n", getpid());

		exit(10);  // 子进程退出, 状态码为 10
	}
	else  // 父进程
	{
		printf("======== 父进程 (PID:%d) 等待子进程 (PID:%d) 执行完毕... ========\n", getpid(), pid);

		pid_t pid = wait(&status);  // 等待子进程执行结束
		if (pid == -1)  // 检查是否失败
		{
			perror("wait()");
			exit(2);
		}

		if (WIFEXITED(status))  // 检查子进程是否正常终止
		{
			printf("======== 父进程: 子进程 (PID:%d) 已正常终止, 退出状态码为: %d ========\n", pid, WEXITSTATUS(status));
		}

		printf("======== 父进程 (PID:%d) 开始执行后续操作 ========\n", getpid());
	}

	return 0;
}
/*
	======== 父进程 (PID:9655) 等待子进程 (PID:9656) 执行完毕... ========
	======== 子进程 (PID:9656) 开始执行 ========
	(10s 后打印)
	======== 子进程 (PID:9656) 进行完毕,准备退出 ========
	======== 父进程: 子进程 (PID:9656) 已正常终止, 退出状态码为: 10 ========
	======== 父进程 (PID:9655) 开始执行后续操作 ========
*/