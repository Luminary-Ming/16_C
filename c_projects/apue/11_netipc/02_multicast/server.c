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
	// AF_INET IPv4 : 协议族
	// SOCK_DGRAM UDP : 报式套接字
	// 0 : 自动选择协议 (UDP)
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == -1)
	{
		perror("socket()");
		return -1;
	}

	// 注意这里不进行 bind 绑定操作, 让内核自动为套接字绑定一个可用的端口和 IP 地址

	// [2] 配置组播选项: 设置套接字从指定的网卡发送组播数据
	struct ip_mreqn imr;
	inet_aton(MULTICAST_IP, &imr.imr_multiaddr);  // 将组播IP地址字符串 (234.2.3.4) 转换为二进制格式, 存入 imr_multiaddr 成员
	inet_aton(LOCAL_IP, &imr.imr_address);  // 将本地 IP 地址字符串 (0.0.0.0) 转换为二进制格式, 存入 imr_address 成员
	imr.imr_ifindex = if_nametoindex(NETCARD_NAME);  // 将网卡名称 (ens33) 转换为网卡索引号, 存入 imr_ifindex 成员

	// [3] 调用 setsockopt 设置套接字选项, 启用组播发送功能
	// IPPROTO_IP : IP协议层
	// IP_ADD_MEMBERSHIP : 加入多播组选项
	// &imr : 组播配置结构体指针, sizeof(imr) : 结构体大小
	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, &imr, sizeof(imr)) == -1)
	{
		perror("setsockopt()");
		close(sd);
		return -2;
	}

	// [4] 配置目标地址 (组播组地址), 准备发送数据
	struct sockaddr_in remote_addr;  // 定义对端地址结构体, 用于存储组播组地址信息
	remote_addr.sin_family = AF_INET;  // 设置 IPv4 协议族
	inet_aton(MULTICAST_IP, &remote_addr.sin_addr);  // 将组播 IP 地址字符串 (234.2.3.4) 转换为二进制格式, 填入 sin_addr 成员
	remote_addr.sin_port = htons(RECV_PORT);  // 将端口号 (2233) 转换为网络字节序, 填入 sin_port 成员

	// [5] 循环向组播组发送数据
	while (1)
	{
		// 向组播地址发送数据
		char msg[] = "今天我很高兴";
		sendto(sd, msg, sizeof(msg) - 1, 0, (struct sockaddr *)&remote_addr, sizeof(remote_addr));
	
		sleep(1);  // 程序暂停 1 秒钟, 控制发送频率
	}

	// [6] 关闭套接字 (实际不会执行到这里, 因为 while(1) 无限循环)
	close(sd);
	return 0;
}