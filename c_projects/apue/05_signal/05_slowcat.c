/*
	cat 命令实现
	cat 读取文件内容, 打印在 stdout 上
*/
#include <stdio.h>
#include <string.h>  // 使用 memset
#include <sys/types.h>  // 使用 open 函数
#include <sys/stat.h>  // 使用 open 函数
#include <fcntl.h>  // 使用 open 函数
#include <unistd.h>  // 使用 read write 函数

#define BUF_SIZE 8192  // 缓冲区大小

int mycat(const char *pathname);

static int flag = 0;  // 用来标记是否执行 IO 操作

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage : %s + filename\n", argv[0]);
		return -1;
	}

	mycat(argv[1]);

	return 0;
}

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

