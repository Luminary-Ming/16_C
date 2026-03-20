/*
	计算文件或目录的磁盘占用空间
	相当于 du -sh 命令
 */
#include <stdio.h>
#include <sys/types.h>  // opendir() closedir() struct stat
#include <dirent.h>  // opendir() closedir()
#include <sys/stat.h>  // struct stat
#include <unistd.h>  // struct stat
#include <errno.h>
#include <string.h>

#define BUFSIZE 1024  // 最大路径长度

static long _mydu_blocks(const char *pathname);  // 递归计算文件或目录占用的磁盘块数
static long mydu(const char *pathname);  // 把获取到的所占磁盘空间大小进行单位转换

int main(int argc, char *argv[])
{
	if (argc < 2)  // 检查命令行参数
	{
		fprintf(stderr, "Usage : %s + filename\n", argv[0]);
		return -1;
	}

	printf("%ldK\t%s\n", mydu(argv[1]), argv[1]);
	
	return 0;
}

// 递归计算文件或目录占用的磁盘块数
static long _mydu_blocks(const char *pathname)
{
	struct stat fs;  // fs 存储获取的文件元信息
	if (stat(pathname, &fs) == -1)  // 获取文件元信息
	{
		perror("stat()");
		return -1;
	}

	if (!S_ISDIR(fs.st_mode))  // 检查是否为目录
	{
		return fs.st_blocks;  // 返回非目录文件的块数
	}

	long count = fs.st_blocks;  // 获取当前目录本身的块数 (. 当前目录)
	DIR *dp = opendir(pathname);  // 打开目录
	if (dp == NULL)
	{
		perror("opendir()");
		return -2;
	}

	char buf[BUFSIZE] = {0};  // 存储文件路径

	while (1)  // 循环读取目录项结构
	{
		errno = 0;  // 调用 readdir() 之前将 errno 设置为零, 用于检测 readdir 错误
		struct dirent *entry = readdir(dp);  // entry 指向目录项结构
		if (entry == NULL)  // 如果返回 NULL, 说明读取目录项结构失败或者结束
		{
			if (errno != 0)  // 如果 errno 变化, 则读取目录项结构失败
			{
				perror("readdir()");
				closedir(dp);
				return -3;
			}
			break;  // 如果 errno 不变, 则读取目录项结构结束
		}
		
		// 跳过当前目录 (已经加过了) 和上一级目录 (没必要算上一级目录)
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
			continue;
		memset(buf, 0, BUFSIZE);  // 清空 buf 空间
		strcpy(buf, pathname);  // 拷贝目录名
		strcat(buf, "/");  // 拼接 `/` 分隔符
		strcat(buf, entry->d_name);  // 拼接子文件名
		count += _mydu_blocks(buf);  // 递归累加子文件的块数
	}
	return count;
}

// 把获取到的所占磁盘空间大小进行单位转换
static long mydu(const char *pathname)
{
	return _mydu_blocks(pathname) / 2;  // 所占磁盘空间的块 / 2 = KB
}

