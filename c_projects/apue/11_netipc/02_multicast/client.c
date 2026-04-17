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

#include "protocol.h"

int main(void)
{
	// [1] 创建 UDP 报式套接字
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == -1)
	{
		perror("socket()");
		return -1;
	}

	// [2] 将本地地址与套接字绑定 (指定接收数据的 IP 和端口)
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;  // 设置 Ipv4 协议族
	// 0.0.0.0 表示绑定所有网络接口, 可以接收任意网卡的数据
	inet_aton(LOCAL_IP, &my_addr.sin_addr);  // 将点分十进制的服务器 IP 字符串转换为网络字节序的二进制 IP 地址
	my_addr.sin_port = htons(RECV_PORT);  // 将本地字节序的端口号转换为网络字节序 (大端字节序)

	// [3] 将套接字绑定到本地 IP 和端口
	if (bind(sd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{
		perror("bind()");
		close(sd);  // 绑定失败, 关闭已创建的套接字文件描述符
		return -2;
	}

	// [4] 加入多播组 (告诉内核我要接收这个组播组的数据)
	struct ip_mreqn imr;  // 存储组播组配置信息
	inet_aton(MULTICAST_IP, &imr.imr_multiaddr);  // 将组播IP地址字符串 (234.2.3.4) 转换为二进制格式, 存入 imr_multiaddr 成员
	inet_aton(LOCAL_IP, &imr.imr_address);  // 将本地IP地址字符串 (0.0.0.0) 转换为二进制格式, 存入 imr_address 成员
	imr.imr_ifindex = if_nametoindex(NETCARD_NAME);  // 将网卡名称 (ens33) 转换为网卡索引号, 存入 imr_ifindex 成员

	// [5] 设置套接字选项, 加入多播组
	// IPPROTO_IP : IP协议层
	// IP_ADD_MEMBERSHIP : 加入多播组选项
	// &imr : 组播配置结构体指针, sizeof(imr) : 结构体大小
	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof(imr)) == -1)
	{
		perror("setsockopt()");
		close(sd);
		return -3;
	}

	// [6] 接收多播组中的消息
	struct data_st buf;
	recvfrom(sd, buf.msg, MSG_SIZE, 0, NULL, NULL);
	// 参数说明 :
	// sd 套接字描述符
	// buf.msg 接收数据缓冲区 (字符数组, 用于存储消息内容)
	// 0 标志位 (默认行为)
	// NULL 不保存发送方地址信息
	// NULL 不接收发送方地址长度

	puts(buf.msg);  // 将接收到的消息打印到标准输出, 自动添加换行符

	// [7] 关闭套接字
	close(sd);  // 成功发送后, 关闭套接字文件描述符, 释放系统资源
	return 0;
}