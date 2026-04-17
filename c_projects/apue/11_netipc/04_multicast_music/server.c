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
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "protocol.h"
#include "tbf.h"

int sd;  // 全局套接字, 用于信号处理
int tbf_id;  // 全局令牌桶描述符

// 信号处理函数
void sig_handler(int none)
{
	printf("\n正在停止播放...\n");
	close(sd);  // 关闭UDP套接字，停止网络通信
	tbf_destroy(tbf_id);  // 销毁令牌桶，释放内存资源
	exit(0);
}

// 使用令牌桶进行流量控制
void send_with_tbf(int sd, struct sockaddr_in *addr, const char *filename);

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: <./server> + <filename>\n");
		return -1;
	}

	// 信号注册
	signal(SIGINT, sig_handler);

	// 初始化令牌桶, 每秒 128KB (128 * 1024), 桶容量 256KB (256 * 1024)
	tbf_id = tbf_init(131072, 262144);
	if (tbf_id < 0)
	{
		fprintf(stderr, "tbf_init() is failed\n");
		return -2;
	}

	// 创建 UDP 报式套接字
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == -1)
	{
		perror("socket()");
		return -3;
	}

	// 注意这里不进行 bind 绑定操作, 让内核自动为套接字绑定一个可用的端口和 IP 地址

	// 配置组播选项: 设置套接字从指定的网卡发送组播数据
	struct ip_mreqn imr;
	inet_aton(MULTICAST_IP, &imr.imr_multiaddr);  // 将组播IP地址字符串 (234.2.3.4) 转换为二进制格式, 存入 imr_multiaddr 成员
	inet_aton(LOCAL_IP, &imr.imr_address);  // 将本地 IP 地址字符串 (0.0.0.0) 转换为二进制格式, 存入 imr_address 成员
	imr.imr_ifindex = if_nametoindex(NETCARD_NAME);  // 将网卡名称 (ens33) 转换为网卡索引号, 存入 imr_ifindex 成员

	// 调用 setsockopt 设置套接字选项, 启用组播发送功能
	// IPPROTO_IP : IP协议层
	// IP_ADD_MEMBERSHIP : 加入多播组选项
	// &imr : 组播配置结构体指针, sizeof(imr) : 结构体大小
	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, &imr, sizeof(imr)) == -1)
	{
		perror("setsockopt()");
		close(sd);
		return -4;
	}

	// 配置目标地址 (组播组地址), 准备发送数据
	struct sockaddr_in remote_addr;  // 定义对端地址结构体, 用于存储组播组地址信息
	remote_addr.sin_family = AF_INET;  // 设置 IPv4 协议族
	inet_aton(MULTICAST_IP, &remote_addr.sin_addr);  // 将组播 IP 地址字符串 (234.2.3.4) 转换为二进制格式, 填入 sin_addr 成员
	remote_addr.sin_port = htons(RECV_PORT);  // 将端口号 (2233) 转换为网络字节序, 填入 sin_port 成员

	printf("正在循环播放音乐: '%s'", argv[1]);

	// 开始发送音乐
	send_with_tbf(sd, &remote_addr, argv[1]);

	close(sd);  // 关闭套接字
	tbf_destroy(tbf_id);
	return 0;
}

void send_with_tbf(int sd, struct sockaddr_in *addr, const char *filename)
{
	FILE *fp = fopen(filename, "rb");  // 以二进制读的方式打开文件
	if (fp == NULL)
	{
		perror("fopen()");
		return;
	}

	struct stat st;
	if (stat(filename, &st) == -1)
	{
		perror("stat()");
		return;
	}
	long total_size = st.st_size;  // 获取文件大小

	printf("音乐大小: %ld 字节\n", total_size);

	struct data_st packet;  // 数据包结构体, 包含序列号、总大小和实际数据
	int seq = 0;  // 数据包序列号, 从 0 开始递增, 用于检测丢包
	int send_first = 1;  // 是否发送第一包 (第一包需要携带文件总大小信息)
	size_t bytes_read;  // 实际读取的字节数

	while (1)  // 音乐无限循环播放
	{
		// 读取数据块
		// 从文件中读取一块数据到 packet.data 缓冲区
		// 每次读 1024 个数据, 每个数据占 1 字节
		bytes_read = fread(packet.data, 1, BLOCK_SIZE, fp);

		if (bytes_read > 0)  // 成功读取
		{
			// 第一包需要发送文件总大小信息给客户端
			if (send_first == 1)
			{
				packet.total_size = total_size;  // 设置总大小字段
				send_first = 0;  // 标记第一包已发送, 后续不再发送总大小
			}
			else
			{
				packet.total_size = 0;  // 非第一包, 设置总大小字段为 0
			}

			packet.seq = seq++;  // 设置序列号自增, 用于检测客户端丢包

			// 令牌桶流量控制机制
			int tokens_needed = BLOCK_SIZE;  // 每次发送 BLOCK_SIZE 个字节数据, 需要消耗 BLOCK_SIZE 个令牌
			int fetched = 0;  // 已经获取到的令牌数
			
			while (fetched < tokens_needed)  // 循环获取, 直到获取足够的令牌
			{
				int token_num = tbf_fetch_token(tbf_id, tokens_needed - fetched);  // 尝试获取令牌
				if (token_num > 0)
					fetched += token_num;  // 累积已获取的令牌数
				else if (token_num == 0)
					usleep(10000);  // 等待 10ms, 给令牌桶补充令牌的时间
				else
				{
					perror("tbf_fetch_token()");
					fclose(fp);
					return;
				}
			}

			// 计算要发送数据包的大小: 序列号(4 byte) + 总数据块大小(4 byte) + 实际数据长度
			int send_len = sizeof(packet.seq) + sizeof(packet.total_size) + bytes_read;

			// 发送 UDP 数据报到组播地址
			if (sendto(sd, &packet, send_len, 0, (struct sockaddr *)addr, sizeof(*addr)) == -1)
			{
				perror("sendto()");
				break;
			}

			// 每发送 10 个数据包显示一次发送进度 (使用 \r 实现同一行刷新)
			if (seq % 10 == 0)
			{
				printf("发送进度: %ld/%ld byte\r", ftell(fp), total_size);  // ftell 获取当前文件位置
				fflush(stdout);  // 刷新缓冲区, 立即显示进度
			}
		}

		// 检测是否到达文件末尾, 文件读取完成
		if (feof(fp))
		{
			printf("\n音乐播放完毕, 单曲循环...");
			rewind(fp);  // 将文件指针重置到文件开头, 实现循环播放
			send_first = 1;  // 重置第一包的标志, 下一轮第一包需要发送文件大小
			seq = 0;  // 重置序列号, 从 0 开始重新计数
			sleep(1);  // 暂停 1s, 避免连续播放之间没有间隔
		}
	}

	fclose(fp);  // 关闭文件指针 (无限循环, 实际不会执行到这里)
}