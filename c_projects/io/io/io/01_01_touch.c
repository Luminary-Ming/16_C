/*
	touch 的实现
	特点 : 文件不存在则创建, 文件已存在只会更新时间戳
*/
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int mytouch(const char *pathname);

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "%s : 缺少要操作的文件\n", argv[0]);
		return -1;
	}

	for (int i = 1; i < argc; i++)
		mytouch(argv[i]);

	return 0;
}

int mytouch(const char *pathname)
{
	int fd = open(pathname, O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
	{
		fprintf(stderr, "创建文件失败 '%s'\n", pathname);
		return -1;
	}
	close(fd);
	return 0;
}