#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 32

int main(void)
{
	int ret = 0;  // 返回状态码
	int pfd[2];  // 存储 pipe 的读端和写端的文件描述符

	if (pipe(pfd) == -1)  // 创建管道
	{
		perror("pipe()");
		ret = -1;
		goto ERR_1;
	}

	pid_t pid = fork();  // 创建子进程
	if (pid < 0)
	{
		perror("fork()");
		ret = -2;
		goto ERR_2;
	}

	char buf[BUF_SIZE];  // 存储读取的数据

	if (pid == 0)  // 子进程
	{
		close(pfd[0]);  // 子进程关闭 pipe 的读端
		write(pfd[1], "hello world!", 12);  // 往 pipe 的写端中写数据
		close(pfd[1]);  // 子进程关闭 pipe 的写端
		exit(0);  // 终止子进程
	}
	else  // 父进程
	{
		close(pfd[1]);  // 父进程关闭 pipe 的写端 (等子进程结束前就可以先关了)
		wait(NULL);  // 等待子进程结束
		read(pfd[0], buf, BUF_SIZE);  // 读取子进程写入的数据
		printf("parent : %s\n", buf);
		close(pfd[0]);  // 父进程关闭 pipe 的读端
		return 0;
	}

ERR_2:
	close(pfd[0]);  // 关闭 pipe 的读端
	close(pfd[1]);  // 关闭 pipe 的写端
ERR_1:
	return ret;
}