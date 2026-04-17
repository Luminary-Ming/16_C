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

int main(void)
{
	// [1] 创建 UDP 报式套接字
	// AF_INET IPv4 : 协议族
	// SOCK_DGRAM UDP : 报式套接字
	// 0 : 自动选择协议 (UDP)
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == -1)
	{
		perror("socket()");
		return -1;
	}

	// [2] 配置本地地址结构体, 准备绑定
	struct sockaddr_in my_addr;  // 存储本地 IP 地址和端口号
	my_addr.sin_family = AF_INET;  // 设置 IPv4
	inet_aton(SERVER_IP, &my_addr.sin_addr);  // 将十进制的服务器 IP 字符串转换为网络字节序的二进制 IP 地址
	my_addr.sin_port = htons(SERVER_PORT);  // 将本地字节序的端口号转为网络字节序 (大端字节序)
	
	// [3] 将套接字绑定到本地 IP 和端口
	if (bind(sd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{
		perror("bind()");
		close(sd);  // 绑定失败, 关闭已创建的套接字文件描述符
		return -2;
	}

	// [4] 循环接收来自客户端的 UDP 数据报
	while (1)  // 无限循环, 持续接收数据
	{
		struct sockaddr_in remote_addr;  // 存储发送端 (客户端) 的 IP 地址和端口号
		socklen_t remote_addr_len = sizeof(remote_addr);
		struct data_st buf;  // 存储接收到的协议数据, 包含 id 和 msg 字段
		
		// [5] 接收 UDP 数据报
		// 0 : 无特殊标志
		ssize_t count = recvfrom(sd, &buf, sizeof(buf), 0,
								 (struct sockaddr *)&remote_addr, &remote_addr_len);
		if (count == -1)
		{
			perror("recvfrom()");
			close(sd);
			return -3;
		}

		printf("\n**********MSG**********\n");
		printf("Remote IP: %s\n", inet_ntoa(remote_addr.sin_addr));  // 将网络字节序的 IP 地址转为点分十进制字符串并打印
		printf("Remote PORT: %d\n", ntohs(remote_addr.sin_port));  // 将网络字节序的端口号转换为主机字节序并打印
		printf("ID : %d\n", buf.id);  // 打印数据包中的ID字段
		printf("MSG : %s\n", buf.msg);  // 打印数据包中的消息内容 (字符串)
		printf("***********************\n");  // 打印消息分隔尾
	}

	// [7] 关闭套接字 (实际不会执行到这里, 因为 while(1) 无限循环)
	close(sd);
	return 0;
}
/*
	**********MSG**********
	Remote IP: 10.11.17.68
	Remote PORT: 43458
	ID : 1
	MSG : nihao
	***********************
*/