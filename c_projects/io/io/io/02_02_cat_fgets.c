/*
    cat 命令实现
*/
#include <stdio.h>
#include <string.h>

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

static int mycat(const char *pathname)
{
    FILE *fp = fopen(pathname, "r");
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
        if (fgets(buf, BUFSIZE, fp) == NULL)  // 判断是否读取完数据
            break;
        fputs(buf, stdout);
    }
    fclose(fp);
    return 0;
}