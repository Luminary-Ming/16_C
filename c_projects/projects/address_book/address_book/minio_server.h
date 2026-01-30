#ifndef MINIO_SERVER_H
#define MINIO_SERVER_H
#include <x86_64-linux-gnu/curl/curl.h>  // curl 头文件

#define ENDPOINT "10.11.17.141"  // ip 地址
#define PORT 9000  // 端口
#define ACCESS_KEY "g0KyMWdYHql3VKrdWFJj"  // 访问密钥
#define SECRET_KEY "7XWT64V22VOnRTQ2Hf0KkeoWaOWFbCFP4Itc8qUY"  // 秘密密钥
#define BUCKET "filebucket/address_book"  // 桶名

// MinIO 服务器配置结构体
typedef struct minio
{
    char endpoint[256];  // ip 地址
    int port;  // 端口
    char access_key[64];  // 访问密钥
    char secret_key[64];  // 秘密密钥
    char bucket[64];  // 桶名
} MinioConfig;

// 创建 MinIO 
MinioConfig *minio_create(void);

// 释放 MinIO 
void minio_destroy(MinioConfig *config);

// 上传文件到 MinIO
int minio_upload(MinioConfig *config, const char *object_name, const char *local_file);

// 生成预览 URL
char *minio_preview_url(MinioConfig *config, const char *object_name);

// 初始化 CURL 库
void minio_init(void);

// 清理 CURL 库
void minio_cleanup(void);

#endif