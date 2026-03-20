/*
	touch 的实现
	特点 : 文件不存在则创建, 文件已存在只会更新时间戳
*/
#include <stdio.h>
#include <sys/types.h>  // 使用 open 函数
#include <sys/stat.h>  // 使用 open 函数
#include <fcntl.h>  // 使用 open 函数
#include <unistd.h>  // 使用 close 函数
#include <utime.h>  // 使用 utime 函数


int mytouch(const char *pathname);

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage : %s + filename\n", argv[0]);
		return -1;
	}

	for (int i = 1; i < argc; i++)
		mytouch(argv[i]);
}

int mytouch(const char *pathname)
{
	// 打开文件, 不存在则创建, 权限 0644 (0644 & ~0002)
	// int fd = open(pathname, O_RDONLY, 0644);  // 这个也行 0100 | 00 = 0100
	int fd = open(pathname, O_CREAT | O_RDONLY, 0644);  // POSIX 规定必须指定一个访问模式 (O_RDONLY O_WRONLY O_RDWR 三者必选其一)
	if (fd < 0)
	{
		fprintf(stderr, "Create Failed '%s'\n", pathname);
		return -2;
	}

	close(fd);

	// 将文件的最后修改时间更新为当前时间
	if (utime(pathname, NULL) == -1)
	{
		fprintf(stderr, "Update Time Failed '%s'\n", pathname);
		return -3;
	}

	return 0;
}

