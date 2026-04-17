#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define SERVER_IP "10.11.17.68"
#define SERVER_PORT 2233

#define MSG_SIZE 256

struct data_st
{
	int8_t id;
	char msg[MSG_SIZE];
}__attribute__((packed));  // 按单字节对齐

#endif

/*
	为什么要按单字节对齐 ?

	按单字节对齐 __attribute__((packed)) 在网络编程中非常重要
	
	[1] 避免结构体填充字节

	默认情况下，编译器会对结构体进行对齐优化 :

	// 不使用 packed 的情况（默认 4 字节对齐）
	struct data_st {
		int8_t id;      //   1 字节
		char msg[256];  // 256 字节
		// 编译器可能在 id 后面填充 3 个无用字节
		// 实际大小可能是 260 字节，而不是 257 字节
	};

	// 使用 packed 后
	struct data_st {
		int8_t id;      //   1 字节
		char msg[256];  // 256 字节
		// 没有填充字节，实际大小就是 257 字节
	}__attribute__((packed));


	[2] 精确控制协议格式

	在网络协议中，数据格式必须严格按照规范定义：
		UDP/TCP 传输的是字节流
		协议文档规定第 1 个字节是 ID，第 2-257 字节是消息内容
		使用packed可以精确匹配协议规范
*/