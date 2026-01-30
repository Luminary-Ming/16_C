/*
	touch 的实现
*/
#include <sys/time.h> 
#include <time.h>
#include <string.h>
#include <stdio.h>


int main(int argc, char *argv[])
{

    const char *object_name = "hello.jpg";

    struct timeval tv;  // 时间结构体(秒, 微秒)
    gettimeofday(&tv, NULL);  // 获取当前时间, 赋值给 tv 结构体
    long timestamp = (long)tv.tv_sec;

    const char *dot = strrchr(object_name, '.');  // 文件名 '.' 的位置

    char prefix[128] = { 0 };  // 文件名前缀
    int len = dot - object_name;  // 前缀长度
    strncpy(prefix, object_name, len);
    prefix[len] = '\0';

    char new_name[256] = { 0 };
    snprintf(new_name, 256, "%s_%ld.%s", prefix, timestamp, dot + 1);

    printf("%s\n", new_name);

}