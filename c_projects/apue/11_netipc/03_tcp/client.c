#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include "protocol.h"

int main(void)
{
	// [1] 创建 TCP 流式套接字
	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_socket == -1)
	{
		perror("socket()");
		return -1;
	}

	// [2] 绑定本地地址到套接字
	struct sockaddr_in raddr;
	raddr.sin_family = AF_INET;  // 设置 IPv4 协议族
	inet_aton(SERVER_IP, &raddr.sin_addr); // 将服务器 IP 地址字符串 (来自 protocol.h) 转换为二进制格式, 填入 sin_addr 成员
	raddr.sin_port = htons(SERVER_PORT);   // 将服务器端口号 (来自 protocol.h) 从主机字节序转换为网络字节序, 填入 sin_port 成员

	// 调用 connect 函数向服务器发起 TCP 连接请求 (三次握手)
	if (connect(tcp_socket, (struct sockaddr *)&raddr, sizeof(raddr)) == -1)
	{
		perror("bind()");
		close(tcp_socket);
		return -2;
	}

	// [3] 连接成功后, 从服务器读取数据 (I/O 操作)
	char msg[MSG_SIZE] = { 0 };

	// 从 TCP 连接中读取服务器发送的数据
	read(tcp_socket, msg, MSG_SIZE);

	puts(msg);  // 将读取到的数据打印到标准输出, puts 会自动添加换行符

	// [4] 关闭TCP流式套接字
	close(tcp_socket); 
	return 0;
}