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

#include "protocol.h"

int main(int argc, char *argv[])
{
	// [0] 检查命令行参数个数
	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <ID> <MSG>\n", argv[0]);
		return -1;
	}

	// [1] 创建 UDP 报式套接字
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == -1)
	{
		perror("socket()");
		return -2;
	}

	// [2] 准备要发送的数据
	struct data_st buf;  // 存储要发送的协议数据，包含 id 和 msg 字段
	buf.id = atoi(argv[1]);
	strncpy(buf.msg, argv[2], MSG_SIZE);
	buf.msg[MSG_SIZE - 1] = '\0';

	// [3] 配置服务器地址结构体 (目标地址)
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;  // 设置 Ipv4 协议
	inet_aton(SERVER_IP, &server_addr.sin_addr);  // 将点分十进制的服务器 IP 字符串转换为网络字节序的二进制 IP 地址
	server_addr.sin_port = htons(SERVER_PORT);  // 将本地字节序的端口号转换为网络字节序 (大端字节序)

	// [4] 计算实际要发送的数据大小
	// 发送大小 = ID 字段大小 (1字节) + 消息字符串实际长度 + 字符串结束符 '\0'(1字节)
	size_t send_size = sizeof(buf.id) + strlen(buf.msg) + 1;  // +1 是为了结束符 '\0'

	// [5] 发送 UDP 数据报到服务器
	// 0 : 无特殊标志
	if (sendto(sd, &buf, send_size, 0,
			   (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)

	{
		perror("sendto()");
		close(sd);
		return -3;
	}

	// [6] 关闭套接字
	close(sd);  // 成功发送后, 关闭套接字文件描述符, 释放系统资源
	return 0;
}