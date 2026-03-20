#define _GNU_SOURCE 
#include <stdio.h>
#include <sys/types.h>  // opendir() closedir() struct stat
#include <dirent.h>  // opendir() closedir()
#include <sys/stat.h>  // struct stat
#include <unistd.h>  // struct stat
#include <errno.h>

// 读取并显示指定目录下的所有条目 (inode 号和文件名)
int main(int argc, char *argv[])
{
	struct stat fs;  // 存储获取文件的元信息
	int ret = 0;  // 返回值, 0 表示成功
	DIR *dp = NULL;  // 目录流指针

	if (argc < 2)  // 检查命令行参数
	{
		fprintf(stderr, "Usage : %s + filename\n", argv[0]);
		ret = -1;
		goto ERR_1;
	}

	if (stat(argv[1], &fs) == -1)  // 获取文件元信息
	{
		perror("stat()");
		ret = -2;
		goto ERR_1;
	}
		
	if (!S_ISDIR(fs.st_mode))  // 检查是否为目录
	{
		printf("%s Not A Directory!\n", argv[1]);
		ret = -3;
		goto ERR_1;
	}

	dp = opendir(argv[1]);  // 打开目录文件
	if (dp == NULL)
	{
		perror("opendir()");
		ret = -4;
		goto ERR_1;
	}

	while (1)  // 循环读取目录项结构
	{
		errno = 0;  // 调用 readdir() 之前将 errno 设置为零, 用于检测 readdir 错误
		struct dirent *entry = readdir(dp);  // entry 指向目录项结构
		if (entry == NULL)  // 如果返回 NULL, 说明读取目录项结构失败或者结束
		{
			if (errno != 0)  // 如果 errno 变化, 则读取目录项结构失败
			{
				perror("readdir()");
				ret = -5;
				goto ERR_2;
			}
			break;  // 如果 errno 不变, 则读取目录项结构结束
		}
		printf("%ld  %s\n", entry->d_ino, entry->d_name);  // 打印子目录的 inode 号以及文件名
	}

ERR_2:
	closedir(dp);  // 关闭目录流
ERR_1:
	return ret;
}

