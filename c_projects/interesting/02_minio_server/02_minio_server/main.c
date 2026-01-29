#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x86_64-linux-gnu/curl/curl.h>
#include <sys/stat.h>

// sudo apt-get install libcurl4-openssl-dev
// gcc main.c -lcurl -o minio_client
// ./minio_client upload test.txt myfile.txt


// 进度回调
static size_t write_to_file(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

static size_t read_from_file(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fread(ptr, size, nmemb, stream);
}

// 上传文件到MinIO
int minio_upload(const char *endpoint, int port,
    const char *access_key, const char *secret_key,
    const char *bucket, const char *object_name,
    const char *local_file)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    struct stat file_info;

    // 打开本地文件
    fp = fopen(local_file, "rb");
    if (!fp)
    {
        perror("无法打开本地文件");
        return -1;
    }

    // 获取文件大小
    if (stat(local_file, &file_info) != 0)
    {
        perror("无法获取文件信息");
        fclose(fp);
        return -1;
    }

    // 初始化curl
    curl = curl_easy_init();
    if (!curl)
    {
        fclose(fp);
        return -2;
    }

    // 构建URL
    char url[1024];
    snprintf(url, sizeof(url), "http://%s:%d/%s/%s",
        endpoint, port, bucket, object_name);

    // 设置curl选项
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, fp);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_from_file);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);

    // 对于MinIO，通常需要以下认证头部
    struct curl_slist *headers = NULL;

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // 执行上传
    printf("正在上传 %s 到 %s...\n", local_file, url);
    res = curl_easy_perform(curl);

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

// 从MinIO下载文件
int minio_download(const char *endpoint, int port,
    const char *access_key, const char *secret_key,
    const char *bucket, const char *object_name,
    const char *local_file)
{

    CURL *curl;
    CURLcode res;
    FILE *fp;

    // 打开本地文件用于写入
    fp = fopen(local_file, "wb");
    if (!fp)
    {
        perror("无法创建本地文件");
        return -1;
    }

    // 初始化curl
    curl = curl_easy_init();
    if (!curl)
    {
        fclose(fp);
        return -2;
    }

    // 构建URL
    char url[1024];
    snprintf(url, sizeof(url), "http://%s:%d/%s/%s",
        endpoint, port, bucket, object_name);

    // 设置curl选项
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_file);

    // 添加认证
    struct curl_slist *headers = NULL;
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Authorization: AWS %s:%s",
        access_key, secret_key);
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // 执行下载
    printf("正在从 %s 下载到 %s...\n", url, local_file);
    res = curl_easy_perform(curl);

    // 清理
    fclose(fp);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "下载失败: %s\n", curl_easy_strerror(res));
        return -3;
    }

    printf("下载成功!\n");
    return 0;
}

// 生成预览URL（简单版本）
char *minio_preview_url(const char *endpoint, int port,
    const char *bucket, const char *object_name)
{

    static char url[1024];
    snprintf(url, sizeof(url), "http://%s:%d/%s/%s",
        endpoint, port, bucket, object_name);
    return url;
}

// 主函数
int main(int argc, char *argv[])
{
    // MinIO配置
    const char *endpoint = "10.11.17.141";
    int port = 9000;
    const char *access_key = "g0KyMWdYHql3VKrdWFJj";
    const char *secret_key = "7XWT64V22VOnRTQ2Hf0KkeoWaOWFbCFP4Itc8qUY";
    const char *bucket = "filebucket";

    if (argc < 2)
    {
        printf("使用方法:\n");
        printf("  上传: %s upload <本地文件> <对象名称>\n", argv[0]);
        printf("  下载: %s download <对象名称> <本地文件>\n", argv[0]);
        printf("  预览: %s preview <对象名称>\n", argv[0]);
        return 1;
    }

    // 初始化curl
    curl_global_init(CURL_GLOBAL_ALL);

    const char *command = argv[1];
    int result = 0;

    if (strcmp(command, "upload") == 0 && argc == 4)
    {
        result = minio_upload(endpoint, port, access_key, secret_key,
            bucket, argv[3], argv[2]);
    }
    else if (strcmp(command, "download") == 0 && argc == 4)
    {
        result = minio_download(endpoint, port, access_key, secret_key,
            bucket, argv[2], argv[3]);
    }
    else if (strcmp(command, "preview") == 0 && argc == 3)
    {
        char *url = minio_preview_url(endpoint, port, bucket, argv[2]);
        printf("预览URL: %s\n", url);
        printf("你可以用浏览器打开这个链接查看文件\n");
    }
    else
    {
        printf("参数错误!\n");
        result = -1;
    }

    curl_global_cleanup();
    return result;
}