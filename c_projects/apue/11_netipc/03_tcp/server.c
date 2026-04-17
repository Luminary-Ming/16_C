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
	struct sockaddr_in laddr;
	laddr.sin_family = AF_INET;  // 设置 IPv4 协议族
	laddr.sin_addr.s_addr = INADDR_ANY; // 设置本地 IP 地址为 INADDR_ANY (0.0.0.0), 表示绑定所有网络接口
	laddr.sin_port = htons(SERVER_PORT);  // 将端口号 (来自 protocol.h) 从主机字节序转换为网络字节序, 这样可以从任何网卡接收客户端连接

	// 调用 bind 函数将套接字与本地地址绑定
	if (bind(tcp_socket, (struct sockaddr *)&laddr, sizeof(laddr)) == -1)
	{
		perror("bind()");
		close(tcp_socket);
		return -2;
	}

	// [3] 将套接字设置为监听状态, 等待客户端连接
	if (listen(tcp_socket, 20) == -1)
	{
		perror("listen()");
		close(tcp_socket);
		return -3;
	}
	
	// [4] 循环接受客户端连接请求 (服务器端通常需要处理多个客户端)
	while (1)
	{ 
		// 接受客户端的连接请求
		// tcp_socket 监听套接字; NULL 不保存客户端地址; NULL 不接收地址长度
		int new_socket = accept(tcp_socket, NULL, NULL);
		if (new_socket == -1)
		{
			perror("accept()");
			close(tcp_socket);
			return -4;
		}

		// [5] 创建子进程来处理客户端的请求 (经典并发服务器模型)
		pid_t pid = fork();  // 创建子进程
		if (pid == -1)
		{
			perror("fork()");
			close(tcp_socket);
			return -5;
		}

		// [6] 连接成功后,进行数据交互 (子进程负责处理, 父进程继续等待新连接)
		if (pid == 0)
		{
			// 子进程向客户端发送数据
			write(new_socket, "T-Rex", 5);

			close(new_socket);  // 子进程关闭客户端套接字, 完成数据交互
			close(tcp_socket);  // 子进程关闭监听套接字 (子进程不需要监听新连接)

			exit(0);  // 子进程正常退出, 状态码为 0
		}

		// 父进程不需要与当前客户端交互, 只需关闭新创建的客户端套接字
		close(new_socket);  // 父进程关闭客户端套接字 (子进程已经复制了该文件描述符)

		// 父进程继续循环, 回到 while 开头, 调用 accept 等待下一个客户端连接
	}

	close(tcp_socket);  // 关闭监听套接字 (实际上由于 while 无限循环, 永远不会执行到这里)
	return 0;
}  