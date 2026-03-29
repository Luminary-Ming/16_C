/*
	定时触发的 cat 命令实现
		功能：读取文件内容并打印到标准输出，通过定时器控制读取频率
		原理：使用 SIGALRM 信号每秒触发一次，每次触发时执行一次读取操作
		作用：实现定时读取，避免连续高速读取，控制输出节奏
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>  // 使用 memset
#include <sys/types.h>  // 使用 open 函数
#include <sys/stat.h>  // 使用 open 函数
#include <fcntl.h>  // 使用 open 函数
#include <unistd.h>  // 使用 read write 函数
#include <signal.h>

#define BUF_SIZE 16  // 缓冲区大小, 每次读取的缓冲区大小 (字节)

static int flag = 0;  // 全局标志位, 1 表示可以执行 IO 操作, 0 表示等待

int mycat(const char *pathname);

/*
	SIGALRM 信号处理函数
	定时触发, 设置标志位允许执行一次 IO 操作
*/
static void sig_handler(int none)
{
	alarm(1);  // 1s 闹钟, 实现 alarm 链
	flag = 1;  // 设置标志位, 通知主循环可以执行 IO 操作
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage : %s + filename\n", argv[0]);
		return -1;
	}

	signal(SIGALRM, sig_handler);  // 给 SIGALARM 信号设置新行为
	alarm(1);  // 1s 闹钟

	mycat(argv[1]);

	return 0;
}

/*
	定时读取文件函数
	每次信号触发时读取固定大小的数据并输出
*/
int mycat(const char *pathname)
{
	int fd = open(pathname, O_RDONLY);  // 以只读的方式打开文件
	if (fd < 0)
	{
		fprintf(stderr, "Open File Failed '%s'\n", pathname);
		return -2;
	}

	char buf[BUF_SIZE] = { 0 };  // 定义缓冲区, 存放读取文件的内容
	while (1)  // 循环读文件内容
	{
		while (!flag);  // 等待信号触发: 死等, 等待标志位变为 1
		flag = 0;  // 清除标志位, 准备下一次等待

		// 也不需要使用 memset 清空缓冲区
		//memset(buf, 0, BUF_SIZE);

		// 从文件描述符中读取数据存到 buf 缓冲区中, 最大读取 BUF_SIZE 字节
		ssize_t read_size = read(fd, buf, BUF_SIZE);  // 获取实际读到的字节数
		if (read_size == -1)  // 如果读到了错误
		{
			perror("Read Failed");
			close(fd);
			return -3;
		}
		else if (read_size == 0)  // 如果读到的字节数为 0, 说明到达文件末尾
			break;  // 读完了退出循环

		// 把 buf 缓冲区中的数据写入到 stdout 中, 一次写 read_size 个数据, 每个数据占 1 字节
		write(STDOUT_FILENO, buf, read_size);
	}

	close(fd);  // 关闭文件描述符
	return 0;
}

