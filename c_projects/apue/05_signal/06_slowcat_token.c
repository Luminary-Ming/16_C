/*
	带流量控制的 cat 命令实现
		功能：读取文件内容并打印到标准输出，使用令牌桶算法控制输出速率
		原理：通过 SIGALRM 信号定时增加令牌，每次读取前需要消耗令牌
		作用：防止程序过快输出，实现流量整形
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>  // 使用 memset
#include <sys/types.h>  // 使用 open 函数
#include <sys/stat.h>  // 使用 open 函数
#include <fcntl.h>  // 使用 open 函数
#include <unistd.h>  // 使用 read write 函数
#include <signal.h>

#define BUF_SIZE 16  // 缓冲区大小, 单次读取的最大字节数
#define CPS BUF_SIZE  // 令牌生成速率, 每秒生成的令牌数 (字节/秒)
#define BURST (CPS) * 20  // 令牌桶容量, 最多可积累的令牌数 (字节) (尽量是速率的整数倍)

int mycat(const char *pathname);

static int token = 0;  // 当前令牌桶中的令牌数量 (字节数)

/*
	SIGALRM 信号处理函数
	定时向令牌桶中添加令牌，实现流量控制
*/
static void sig_handler(int none)
{
	alarm(1);  // 1s 闹钟, 实现 alarm 链

	token += CPS;  // 每当 SIGALARM 信号到来, 按固定速率添加令牌
	if (token >= BURST)  // 令牌数不能超过桶容量上限 
		token = BURST;  // 把令牌定格在上限值
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
	带流量控制的文件读取函数
	使用令牌桶算法控制读取速度
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
		int n = 0;  // 本次计划读取的字节数 (受令牌数限制)

		// 等待令牌,: 如果没有令牌, 则暂停等待令牌
		while (token == 0)  
			pause();  // 等待令牌

		// 消耗令牌: 根据当前令牌数决定本次读取多少字节
		if (token >= CPS)  // 检查令牌桶中令牌数是否大于速率 (想要取出令牌的数量)
			n = CPS;  // 令牌充足, 按最大速率读取
		else
			n = token;  // 令牌不足, 有多少读多少

		token -= n;  // 更新令牌桶中的 token 数量: 从桶中移除已消耗的令牌

		// 也不需要使用 memset 清空缓冲区
		//memset(buf, 0, BUF_SIZE);

		// 从文件描述符中读取数据存到 buf 缓冲区中, 最大读取 n 字节 (受令牌数限制)
		ssize_t read_size = read(fd, buf, n);  // 获取实际读到的字节数
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

