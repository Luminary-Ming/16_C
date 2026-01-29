#include "minio_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// 进度回调函数
static size_t read_from_file(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fread(ptr, size, nmemb, stream);
}

// 创建 MinIO
MinioConfig *minio_create(void)
{
    MinioConfig *config = malloc(sizeof(MinioConfig));
    if (!config)
        return NULL;

    // 初始化所有字段
    memset(config, 0, sizeof(MinioConfig));

    config->port = PORT;
    strcpy(config->endpoint, ENDPOINT);
    strcpy(config->access_key, ACCESS_KEY);
    strcpy(config->secret_key, SECRET_KEY);
    strcpy(config->bucket, BUCKET);

    return config;
}

// 释放 MinIO
void minio_destroy(MinioConfig *config)
{
    if (!config)
        return;

    free(config);
}

// 上传文件到 MinIO
int minio_upload(MinioConfig *config, const char *object_name, const char *local_file)
{
    if (!config || !object_name || !local_file)
    {
        fprintf(stderr, "参数错误\n");
        return -1;
    }

    // 打开本地文件
    FILE *fp = fopen(local_file, "rb");  // rb 以二进制读文件
    if (!fp)
    {
        perror("无法打开本地文件");
        return -1;
    }

    // 获取文件大小
    struct stat file_info;
    if (stat(local_file, &file_info) != 0)
    {
        perror("无法获取文件信息");
        fclose(fp);
        return -1;
    }

    // 初始化curl
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        fclose(fp);
        return -2;
    }

    // 构建URL
    char url[1024];
    snprintf(url, sizeof(url), "http://%s:%d/%s/%s", config->endpoint, config->port, config->bucket, object_name);

    // 设置curl选项
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, fp);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_from_file);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);

    // 添加必要的 HTTP 头
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // 执行上传
    printf("正在上传 %s 到 %s...\n", local_file, url);
    CURLcode res = curl_easy_perform(curl);

    // 清理
    fclose(fp);
    if (headers)
        curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "上传失败: %s\n", curl_easy_strerror(res));
        return -3;
    }

    printf("上传成功!\n");
    return 0;
}

// 生成预览URL
char *minio_preview_url(MinioConfig *config, const char *object_name)
{
    if (!config || !object_name)
    {
        return NULL;
    }

    char *url = malloc(1024);
    if (!url) return NULL;

    snprintf(url, 1024, "http://%s:%d/%s/%s", config->endpoint, config->port, config->bucket, object_name);

    return url;
}

// 初始化 CURL 库
void minio_init(void)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

// 清理 CURL 库
void minio_cleanup(void)
{
    curl_global_cleanup();
}


