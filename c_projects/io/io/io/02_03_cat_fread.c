/*
    cat 命令实现
*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>  // 使用 open 函数
#include <sys/stat.h>  // 使用 open 函数
#include <fcntl.h>  // 使用 open 函数

#define BUFSIZE 4096

static int mycat(const char *pathname);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage : %s + filename\n", argv[0]);
        return -1;
    }

    mycat(argv[1]);

    return 0;
}

 int mycat(const char *pathname)
{
    int *fd = open(pathname, O_RDONLY);
    if (fp == NULL)
    {
        perror("fopen()");
        return -1;
    }


    int ch = 0;
    char buf[BUFSIZE];  // 缓冲区
    while (1)
    {
        memset(buf, 0, BUFSIZE); // 清空脏数据
        if (fread(buf, 1, BUFSIZE, fp) == 0)  // 判断是否读取完数据
            break;
        fwrite(buf, 1, BUFSIZE, stdout);
    }
    fclose(fp);
    return 0;
}