#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>  // sleep 函数头文件
#include "contact.h"
#include "contact_llist.h"
#include "db_mysql.h"
#include "http_server.h"

// 信号处理函数
void signal_handler(int sig)
{
    printf("\nReceived signal %d, shutting down...\n", sig);
    http_server_stop();
    exit(0);
}

int main(int argc, char *argv[])
{
    // 设置信号处理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("=== 通讯录后端接口 Api ===\n");

    // 启动 HTTP 服务器（默认端口8080）
    int port = 8080;
    if (argc > 1)
    {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535)
        {
            printf("Invalid port number: %s\n", argv[1]);
            printf("Using default port 8080\n");
            port = 8080;
        }
    }

    if (!http_server_start(port))
    {
        printf("Failed to start server on port %d\n", port);
        return 1;
    }

    printf("\nServer is running. Press Ctrl+C to stop.\n");

    // 保持程序运行
    while (1)
    {
        sleep(1);  // 保持进程运行
    }

    return 0;
}