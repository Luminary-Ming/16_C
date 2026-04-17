#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define MULTICAST_IP "234.2.3.4"  // 组播 IP 地址, 用于组播通信
#define LOCAL_IP "0.0.0.0"  // 本地 IP 地址, 0.0.0.0 表示绑定所有网络接口
#define NETCARD_NAME "ens33"  // 网卡名称
#define RECV_PORT 2233  // 接收端口

#define BLOCK_SIZE 1024  // 每次发送的数据块大小

// 数据包结构
struct data_st
{
	int32_t seq;  // 序列号, 用于检查丢包
	int32_t total_size;  // 总数据块大小 (第一包发送)
	char data[BLOCK_SIZE];  // 实际数据
}__attribute__((packed));  // 单字节对齐

#endif

/*
	
需求:
	组长将本地一个音乐文件读出来发送到多播组中, 要求同小组的成员任意时刻运行程序都可以收听到多播组中的音频文件
	音乐循环播放; 流量控制 (令牌桶); 客户端创建子进程, 父进程从多播组里面读出数据, 让子进程做进程替换, 进行播放
	
主要特点:
	循环播放：组长程序会循环播放音乐文件

	任意时刻加入：组员可以随时启动并接收音乐

	流量控制：使用令牌桶算法控制发送速率（约 128KB/s）

	进程间通信：使用匿名管道将数据从父进程传递给子进程

	子进程播放：子进程使用 mpv/ffplay/aplay 播放音乐

	进度显示：显示发送和接收进度

	信号处理：支持 Ctrl+C 优雅退出

注意事项:
	确保组长和组员的网卡名称一致 (默认 ens33)
	确保防火墙允许 UDP 多播通信
*/