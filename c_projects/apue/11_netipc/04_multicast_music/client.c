#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h> 

#include "protocol.h"


int sd;  // UDP 套接字描述符
int pipe_fd[2];  // 匿名管道文件描述符: pipe_fd[0]读端, pipe_fd[1]写端

void sig_handler(int none)
{
	printf("\n正在退出...\n");
	close(sd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exit(0);
}

// 播放器进程函数 - 子进程执行的函数
void player_process(int read_fd);

// 接收并播放音乐 - 父进程执行的函数
void receive_and_play();

int main(void)
{
	// 信号处理注册
	signal(SIGINT, sig_handler);

	// 创建匿名管道
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe()");
		return -1;
	}

	// 创建子进程
	pid_t pid = fork();  
	if (pid == -1)
	{
		perror("fork()");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return -2;
	}

	if (pid == 0)  // 子进程
	{
		// 子进程负责播放音乐
		close(pipe_fd[1]);  // 关闭管道的写端
		player_process(pipe_fd[0]);  // 调用播放器函数 (进程替换)
		exit(0);
	}

	// 父进程负责接收网络数据
	close(pipe_fd[0]);  // 关闭管道的读端

	// 创建 UDP 套接字, 用于接收组播数据
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == -1)
	{
		perror("socket()");
		return -3;
	}

	// 将本地地址与套接字绑定 (指定接收数据的 IP 和端口)
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;  // 设置 Ipv4 协议族
	// 0.0.0.0 表示绑定所有网络接口, 可以接收任意网卡的数据
	inet_aton(LOCAL_IP, &my_addr.sin_addr);  // 将点分十进制的服务器 IP 字符串转换为网络字节序的二进制 IP 地址
	my_addr.sin_port = htons(RECV_PORT);  // 将本地字节序的端口号转换为网络字节序 (大端字节序)

	// 将套接字绑定到本地 IP 和端口
	if (bind(sd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{
		perror("bind()");
		close(sd);  // 绑定失败, 关闭已创建的套接字文件描述符
		return -4;
	}

	// 加入多播组 (告诉内核我要接收这个组播组的数据)
	struct ip_mreqn imr;  // 存储组播组配置信息
	inet_aton(MULTICAST_IP, &imr.imr_multiaddr);  // 将组播IP地址字符串 (234.2.3.4) 转换为二进制格式, 存入 imr_multiaddr 成员
	inet_aton(LOCAL_IP, &imr.imr_address);  // 将本地IP地址字符串 (0.0.0.0) 转换为二进制格式, 存入 imr_address 成员
	imr.imr_ifindex = if_nametoindex(NETCARD_NAME);  // 将网卡名称 (ens33) 转换为网卡索引号, 存入 imr_ifindex 成员

	// 设置套接字选项, 加入多播组
	// IPPROTO_IP : IP协议层
	// IP_ADD_MEMBERSHIP : 加入多播组选项
	// &imr : 组播配置结构体指针, sizeof(imr) : 结构体大小
	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof(imr)) == -1)
	{
		perror("setsockopt()");
		close(sd);
		return -5;
	}

	printf("等待播放音乐...\n\n");

	// 接收并播放音乐 (进入循环, 直到收到中断信号)
	receive_and_play();

	wait(NULL);  // 等待子进程退出 (有循环, 实际不会执行到这里)

	// 清理资源
	close(sd);  // 关闭套接字
	close(pipe_fd[1]);  // 关闭管道的写端
	return 0;
}

// 播放器进程函数 - 子进程执行的函数
void player_process(int read_fd)
{
	// 把 stdin 重定向到管道的读端
	dup2(read_fd, STDIN_FILENO);
	close(read_fd);  // 关闭原来的管道读端 (因为已经复制(重定向)到 stdin 了)

	// 执行外部播放程序, 替换当前进程代码段 ("-" 从 stdin 中读取数据)
	execlp("mplayer", "mplayer", "-", NULL);

	perror("execlp()");
	exit(1);
}

// 接收并播放音乐 - 父进程执行的函数
void receive_and_play()
{
	struct data_st packet;  // 数据包结构体, 存储接收的数据
	struct sockaddr_in src_addr;  // 发送方地址结构体
	socklen_t addr_len = sizeof(src_addr);  // 地址结构体长度

	int expected_seq = 0;  // 期望的下一包序列号, 用于检测丢包
	int file_size = 0;  // 音乐文件总大小, 从第一包获取
	int received_bytes = 0;  // 已接收的数据字节数
	int last_progress = -1;  // 上次显示的进度百分比, 避免重复刷新

	printf("开始接收音乐流...\n");

	while (1)
	{
		memset(&packet, 0, sizeof(packet));  // 清空数据包结构体, 避免残留数据

		// 接收 UDP 数据报
		ssize_t recv_len = recvfrom(sd, &packet, sizeof(packet), 0, (struct sockaddr *)&src_addr, &addr_len);
		if (recv_len == -1)
		{
			perror("recvfrom()");
			continue;
		}
	
		// 丢包检测: 检查序列号是否连续 (期望序列号应该等于收到的序列号)
		// expected_seq != 0 表示不是第一包 (第一包从 0 开始)
		if (packet.seq != expected_seq && expected_seq != 0)
		{
			printf("警告: 丢包检测 (期望 %d, 收到 %d)", expected_seq, packet.seq);
		}

		expected_seq = expected_seq + 1;  // 更新期望的下一包序列号

		// 计算实际数据大小: 接收长度减去包头长度 (seq 和 total_size 各占 4 字节)
		int data_size = recv_len - sizeof(packet.seq) - sizeof(packet.total_size);
		if (data_size > 0)  // 有效数据
		{
			// 将接收到的音频数据写入管道
			if (write(pipe_fd[1], packet.data, data_size) == -1)
			{
				perror("write()");
				break;
			}

			received_bytes += data_size;  // 累加已接收字节数

			// 显示接收进度
			if (file_size > 0)
			{
				int progress = (received_bytes * 100) / file_size;  // 计算进度百分比
				if (progress != last_progress)
				{
					printf("接收进度: %d%% (%d/%d 字节)\r", progress, received_bytes, file_size);
					fflush(stdout);  // 刷新缓冲区
					last_progress = progress;  // 更新最后显示的进度
				}
			}
		}

		// 文件接收完成检测: 序列号回到 0 (组长重新开始播放) 且已接收数据 >0
		if (packet.seq == 0 && received_bytes > 0)
		{
			printf("\n音乐播放完成, 等待下一轮...\n");
			received_bytes = 0;  // 重置已接收字节数
			file_size = 0;  // 重置文件大小 (等待第一包重新获取)
			expected_seq = 0;  // 重置期望序列号
			last_progress = -1;  // 重置进度显示
		}
	}
}