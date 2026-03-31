/*
	argv[1] 是一个音乐文件 (.mp3)
	子进程运行一个播放器, 来播放父进程从 argv[1] 文件中读取的数据

	通过管道将音频文件传递给播放器播放
	父进程读取音频文件, 通过管道发送给子进程, 子进程运行 mplayer 播放器来播放音频

	子进程运行一个播放器->进程替换
	下载安装 mplayer 播放器
	sudo apt-get install mplayer
	mplayer + 歌名 播放音乐

	运行测试
	./a.out ~/音乐/"Creepin' Up On You - Darren Hayes.mp3"
*/
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
		count = read(rfd, buf, BUF_SIZE);  // 从 rfd 中读取数据
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
	if (fd < 0)
	{
		perror("open()");
		ret = -2;
		goto ERR_1;
	}

	if (pipe(pfd) == -1)  // 创建管道
	{
		perror("pipe()");
		ret = -1;
		goto ERR_2;
	}

	pid_t pid = fork();  // 创建子进程
	if (pid < 0)
	{
		perror("fork()");
		ret = -2;
		goto ERR_3;
	}

	if (pid == 0)  // 子进程 (播放器从管道中读取数据播放音乐)
	{
		close(pfd[1]);  // 关闭不需要的写端

		// stdin 重定向到 pipe 的读端
		// 此后, 子进程的 stdin 不再从键盘读取
		// 而是从管道读取父进程发来的数据
		if (dup2(pfd[0], STDIN_FILENO) == -1)
		{
			perror("dup2()");
			close(pfd[0]);  // 关闭 pipe 的读端
			exit(1);
		}

		close(pfd[0]);  // 重定向后可以关闭原文件描述符

		// "-" 参数表示 mplayer 将从标准输入 stdin 中读取数据
		// 由于 stdin 已重定向到管道, mplayer 会从管道读取音频数据并播放
		execl("/usr/bin/mplayer", "mplayer", "-", NULL);  // 进程替换, 播放从 stdin 中读取的数据
	
		// 如果 execl 失败，执行到这里
		perror("execl()");
		exit(2);  // 终止子进程
	}
	else  // 父进程 (读取音频文件, 写入管道的写端)
	{
		close(pfd[0]);  // 关闭不需要的读端

		cpfile(fd, pfd[1]);  // 从文件中读取数据并写入管道

		close(pfd[1]);  // 关闭写端

		wait(NULL);  // 等待子进程结束

		close(fd);  // 关闭 argv[1] 文件

		return 0;
	}

ERR_3:
	close(pfd[0]);  // 关闭 pipe 的读端
	close(pfd[1]);  // 关闭 pipe 的写端

ERR_2:	
	close(fd);  // 关闭 argv[1] 文件

ERR_1:
	return ret;
}

