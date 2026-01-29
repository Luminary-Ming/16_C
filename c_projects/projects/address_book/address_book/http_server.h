#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H

#include <microhttpd.h>  // sudo apt-get install libmicrohttpd-dev -y

// HTTP服务器结构
typedef struct
{
    struct MHD_Daemon *daemon;  // HTTP 服务器实例, 创建后服务器自动开始监听端口
    int port;  // 端口
    int running;  // 运行状态
} HttpServer;

// 服务器控制函数
int http_server_start(int port);
void http_server_stop();

// 回调函数声明
int handle_request(void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls);

#endif