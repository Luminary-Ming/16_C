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
    http_server_stop();  // 关闭 http 服务器
    exit(0);  // 退出程序
}

int main(int argc, char *argv[])
{
    // 设置信号处理
    signal(SIGINT, signal_handler);  // Ctrl+C
    signal(SIGTERM, signal_handler);  // kill, systemctl stop, poweroff

    printf("=== 通讯录后端接口 Api ===\n");

    
    int port = 8080;  // 默认端口8080
    if (argc > 1)
    {
        port = atoi(argv[1]);  // 自定义端口
        if (port <= 0 || port > 65535)
        {
            printf("Invalid port number: %s\n", argv[1]);
            printf("Using default port 8080\n");
            port = 8080;  // 无效端口, 使用默认端口
        }
    }

    if (!http_server_start(port))  // 启动 http 服务
    {
        printf("Failed to start server on port %d\n", port);
        return 1;
    }

    printf("\nServer is running. Press Ctrl+C to stop.\n");

    // 保持程序后台运行
    while (1)
    {
        //sleep(1);  // 保持进程运行
        pause();  // 挂起进程，直到收到信号
    }

    return 0;
}