#include <stdio.h>
#include <stdlib.h>

FILE *log_file = NULL;
void *buffer = NULL;

void close_log_file(void)
{
    if (log_file)
    {
        fprintf(log_file, "程序结束，关闭日志\n");
        fclose(log_file);
        printf("日志文件已关闭\n");
    }
}

void free_buffer(void)
{
    if (buffer)
    {
        free(buffer);
        printf("内存缓冲区已释放\n");
    }
}

int main(void)
{
    // 注册清理函数
    if (atexit(close_log_file) != 0)
    {
        fprintf(stderr, "注册日志关闭函数失败\n");
        return -1;
    }

    if (atexit(free_buffer) != 0)
    {
        fprintf(stderr, "注册内存释放函数失败\n");
        return -2;
    }

    // 分配资源
    log_file = fopen("app.log", "w");
    buffer = malloc(1024);

    if (!log_file || !buffer)
    {
        fprintf(stderr, "资源分配失败\n");
        return -3;
    }

    fprintf(log_file, "程序开始运行\n");
    printf("程序正常运行中...\n");

    // 无论在哪里 return 或 exit，清理函数都会执行
    return 0;
}

