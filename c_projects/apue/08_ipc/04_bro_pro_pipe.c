/*
	 1号子进程执行 ps -ajx 
	 2号子进程执行 grep "systemd"

	 ps -ajx | grep "systemd"

	 管道符 '|' 作用用于连接两个命令, 将前一个命令的输出作为后一个命令的输入
	 左边命令 (ps) 的标准输出 (stdout) 作为右边命令 (grep) 的标准输入 (stdin)

	        ps -ajx 使用 pipe 写端: 因为 ps 命令的功能是输出进程信息到标准输出 stdout 上
	 grep "systemd" 使用 pipe 读端: 因为 grep 命令的功能是读取输入, 过滤后输出

	 在管道中: ps → 管道 → grep
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
	int ret = 0;
	int pfd[2];

	if (pipe(pfd) == -1)  // 创建管道
	{
		perror("pipe()");
		ret = -1;
		goto ERR_1;
	}

	pid_t pids[2];  // 存储子进程 pid
	int created_count = 0;  // 记录成功创建的子进程数量

	for (int i = 0; i < 2; i++)  // 循环创建子进程
	{
		pids[i] = fork();  // 创建子进程 
		if (pids[i] < 0)
		{
			perror("fork()");
			ret = -2;
			goto ERR_2;
		}

		created_count++;  // 成功创建的子进程数加1

		if (pids[i] == 0)  // 子进程
		{
			if (i == 0)  // 1 号子进程 (执行 ps -ajx)
			{

				close(pfd[0]);  // 关闭读端

				dup2(pfd[1], STDOUT_FILENO);  // 把 stdout 重定向到 pipe 的写端

				close(pfd[1]);  // 关闭写端

				execl("/usr/bin/ps", "ps", "-ajx", NULL);
				perror("execl()");
				exit(1);
			}

			else if (i == 1) // 2 号子进程 (执行 grep "systemd")
			{
				close(pfd[1]);  // 关闭写端

				dup2(pfd[0], STDIN_FILENO);  // 把 stdin 重定向到 pipe 的读端

				close(pfd[0]);  // 关闭读端

				execl("/usr/bin/grep", "grep", "systemd", NULL);
				perror("execl()");
				exit(2);
			}
		}
	}

	// 父进程关闭管道两端
	close(pfd[0]);
	close(pfd[1]);

	// 等待所有子进程结束
	for (int i = 0; i < created_count; i++)
		waitpid(pids[i], NULL, 0);
	
	return 0;  // 正常结束，直接返回

ERR_2:
	if (pfd[0] >= 0) close(pfd[0]);
	if (pfd[1] >= 0) close(pfd[1]);

	// 等待所有子进程结束, 避免僵尸进程
	for (int i = 0; i < created_count; i++)
		waitpid(pids[i], NULL, 0);

ERR_1:
	return ret;
}

