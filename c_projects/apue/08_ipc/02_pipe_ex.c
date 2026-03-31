#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 128

static int cpfile(int rfd, int wfd)
{
	char buf[BUF_SIZE];  // 存储读取的数据
	int count = 0;  // 存储成功读取的字节数

	while (1)
	{
		count = read(rfd, buf, BUFSIZ);  // 从 rfd 中读取数据
		if (count == -1)  // 读取失败
		{
			perror("read()");
			return -1;
		}
		if (count == 0)  // 读到了结尾
			break;
		write(wfd, buf, count);  // 把 buf 中的数据写到 wfd 中
	}
}

/*
	练习: 父进程读 argv[1] 文件, 发送给子进程, 子进程接收到之后再写到标准输出中
*/
int main(int argc, char *argv[])
{
	int ret = 0;  // 返回状态码
	int pfd[2];  // 存储 pipe 的读端和写端的文件描述符

	if (argc < 2)
	{
		fprintf(stderr, "Usage : %s + filename\n", argv[0]);  // 打印使用说明
		goto ERR_1;
	}

	int fd = open(argv[1], O_RDONLY);  // 打开 argv[1] 文件
	if(fd < 0)
	{
		perror("open()");
		ret = -2;
		goto ERR_1;
	}

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

	if (pid == 0)  // 子进程
	{
		close(pfd[1]);  // 子进程关闭 pipe 的写端
		cpfile(pfd[0], STDOUT_FILENO);  // 从 pipe 的读端中读数据, 写到 stdout 中
		close(pfd[0]);  // 子进程关闭 pipe 的读端
		exit(0);  // 终止子进程
	}
	else  // 父进程
	{
		close(pfd[0]);  // 父进程关闭 pipe 的读端 (等子进程结束前就可以先关了)
		cpfile(fd, pfd[1]);  // 从 argv[1] 文件中读数据, 写到 pipe 的写端
		close(pfd[1]);  // 父进程关闭 pipe 的写端
		wait(NULL);  // 等待子进程结束
		
		close(fd);  // 关闭 argv[1] 文件
		return 0;
	}

ERR_2:
	close(pfd[0]);  // 关闭 pipe 的读端
	close(pfd[1]);  // 关闭 pipe 的写端
	close(fd);  // 关闭 argv[1] 文件
ERR_1:
	return ret;
}

