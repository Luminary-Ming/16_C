#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define MULTICAST_IP "234.2.3.4"  // 组播 IP 地址, 用于组播通信
#define LOCAL_IP "0.0.0.0"  // 本地 IP 地址, 0.0.0.0 表示绑定所有网络接口
#define NETCARD_NAME "ens33"  // 网卡名称
#define RECV_PORT 2233  // 接收端口

#define MSG_SIZE 256

struct data_st
{
	int8_t id;
	char msg[MSG_SIZE];
}__attribute__((packed));  // 单字节对齐

#endif

/*
	为什么是 234.2.3.4 而不是其他组播地址 ? —— 这取决于用途

	整个组播地址范围 224.0.0.0 ~ 239.255.255.255 被划分成不同用途的区域
	234.0.0.0/8 这个块 (234.0.0.0 ~ 234.255.255.255) 被 IANA (互联网号码分配机构) 标记为 "Unicast-Prefix-based IPv4 Multicast Addresses", 即 "基于单播前缀的 IPv4 组播地址"

	简单说: 234.x.x.x 是专门留给 "源特定组播 (SSM)" 之外的、需要与某个具体单播网段关联的组播应用使用的
*/