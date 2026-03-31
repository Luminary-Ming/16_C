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

	int fd = open("./hello", O_RDWR | O_CREAT | O_TRUNC, 0666);  // 打开文件
	if (fd < 0)  // 判断打开文件是否失败
	{
		perror("open()");
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
		write(fd, "hello world!", 12);  // 往 fd 指向的文件中写数据
		close(fd);  // 关闭文件
		exit(0);  // 终止子进程
	}
	else  // 父进程
	{
		wait(NULL);  // 等待子进程结束
		lseek(fd, SEEK_SET, 0);  // 把 fd 指向的文件定位到文件开头
		read(fd, buf, BUF_SIZE);  // 读取子进程写入的数据
		printf("parent : %s\n", buf);
	}
	
ERR_2:
	close(fd);
ERR_1:
	return ret;
}