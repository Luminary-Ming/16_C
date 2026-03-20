/*
	获取文件的元信息
*/
#define _GNU_SOURCE 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

#define BUFSIZE 10
#define TIMESIZE 32

static char get_file_type(mode_t st_mode);  // 获取文件的类型
static char *get_file_permission(mode_t st_mode, char *buf);  // 获取文件的权限
static char *get_file_uname(uid_t st_uid);  // 获取文件的所有者
static char *get_file_gname(uid_t st_gid);  // 获取文件的所属组
static char *get_file_mtime(time_t tm, char *tbuf);  // 获取文件的最后修改内容时间

int main(int argc, char *argv[])
{
	struct stat fs;  // 存储获取文件的元信息
	char buf[BUFSIZE];  // 存储文件权限的字符串
	char tbuf[TIMESIZE];  // 存储文件最后修改内容时间的字符串

	if (argc < 2)
	{
		fprintf(stderr, "Usage : %s + filename\n", argv[0]);
		return -1;
	}

	if (stat(argv[1], &fs) == -1)  // 如果获取文件的元信息失败
	{
		perror("stat()");
		return -2;
	}

	printf("设备编号: %lu\n", fs.st_dev);  // 设备编号
	printf("文件类型: %c\n", get_file_type(fs.st_mode));  // 文件类型
	printf("文件权限: %s\n", get_file_permission(fs.st_mode, buf));  // 文件权限
	printf("文件所有者: %s\n", get_file_uname(fs.st_uid));  // 文件所有者
	printf("文件所属组: %s\n", get_file_gname(fs.st_gid));  // 文件所属组
	printf("硬链接数: %lu\n", fs.st_nlink);  // 硬链接数
	printf("字节大小: %ld\n", fs.st_size);  // 文件字节大小
	printf("最后修改内容时间: %s\n", get_file_mtime(fs.st_mtime, tbuf));  // 文件最后修改内容时间
	printf("文件名: %s\n", argv[1]);  // 文件名

	return 0;
}

// 获取文件的类型
static char get_file_type(mode_t st_mode)  
{
	char c = 0;  // 用来存储解析出来的文件类型的符合

	switch (st_mode & __S_IFMT)
	{
		case __S_IFREG:  c = '-'; break;  // 普通文件
		case __S_IFDIR:  c = 'd'; break;  // 目录文件
		case __S_IFCHR:  c = 'c'; break;  // 字符设备文件
		case __S_IFBLK:  c = 'b'; break;  // 块设备文件
		case __S_IFIFO:  c = 'p'; break;  // 管道文件
		case __S_IFSOCK: c = 's'; break;  // 网络套接字文件
		case __S_IFLNK:  c = 'l'; break;  // 符号链接文件
		default: c = '?'; break;
	}
	return c;
}

// 获取文件的权限
static char *get_file_permission(mode_t st_mode, char *buf)
{
	// 普通权限的掩码
	int mask[BUFSIZE - 1] = { S_IRUSR, S_IWUSR, S_IXUSR, 
							S_IRGRP, S_IWGRP, S_IXGRP,
							S_IROTH, S_IWOTH, S_IXOTH };

	// 特殊权限标志
	int special_mask = st_mode & (S_ISUID | S_ISGID | S_ISVTX);

	char permission[BUFSIZE] = "rwxrwxrwx";  // 满权限字符串

	// 处理普通权限
	for (int i = 0; i < BUFSIZE - 1; i++)
	{
		if (!(st_mode & mask[i]))
			permission[i] = '-';  // 没有权限的位置填充 -
	}

	// 处理特殊权限
	if (special_mask & S_ISUID)  // setuid
	{
		if (permission[2] == 'x')
			permission[2] == 's';
		else
			permission[2] == 'S';
	}

	if (special_mask & S_ISGID)  // setgid
	{
		if (permission[5] == 'x')
			permission[5] == 's';
		else
			permission[5] == 'S';
	}

	if (special_mask & S_ISVTX)  // sticky bit
	{
		if (permission[8] == 'x')
			permission[8] == 't';
		else
			permission[8] == 'T';
	}

	strncpy(buf, permission, BUFSIZE);  // 把局部数组的数据转存到形参指向的存储空间

	return buf;
}

// 获取文件的所有者
static char *get_file_uname(uid_t st_uid)
{
	struct passwd *p = getpwuid(st_uid);  // 通过 UID 获取用户信息
	if (p == NULL)
	{
		perror("getpwuid()");
		return NULL;
	}

	return p->pw_name;
}

// 获取文件的所属组
static char *get_file_gname(uid_t st_gid)
{
	struct group *p = getgrgid(st_gid);  // 通过 GID 该组的完整信息
	if (p == NULL)
	{
		perror("getgrgid()");
		return NULL;
	}

	return p->gr_name;
}

// 获取文件的最后修改内容时间
static char *get_file_mtime(time_t tm, char *tbuf)
{
	struct tm *p = localtime(&tm);  // p 指向转换后的时间结构体
	if (p == NULL)  // 如果转换失败
	{
		perror("localtime()");
		return NULL;
	}

	strftime(tbuf, TIMESIZE, "%02m月 %d %H:%M", p);

	return tbuf;
}

