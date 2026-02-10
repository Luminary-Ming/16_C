/*
	实现 cp 命令, 拷贝文件 - fgets 版本
	cp 源文件 目标文件(不存在则创建, 存在则覆盖)
*/

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 4096  // 定义缓冲区大小

static int mycp(const char *srcfile, const char *destfile);

int main(int argc, char *argv[])
{
	if (argc < 3)  // 判断命令行参数的个数是否少于 3 个
	{
		fprintf(stderr, "Usage : %s + srcfile + destfile\n", argv[0]);  // 打印使用说明
		return -1;  // 由于令行参数的个数少于 3 个,结束程序,并且返回 -1
	}

	mycp(argv[1], argv[2]);

	return 0;
}

static int mycp(const char *srcfile, const char *destfile)
{
	FILE *src_fp = fopen(srcfile, "r");  // 读取 src 文件中的内容
	if (src_fp == NULL)
	{
		perror("src_fp_fopen()");
		return -1;
	}

	FILE *dest_fp = fopen(destfile, "w");  // 向 dest_fp 文件中写入内容
	if (dest_fp == NULL)
	{
		fclose(src_fp);  // 关闭文件流
		perror("dest_fp_fopen()");
		return -2;
	}

	char buffer[BUFFER_SIZE];  // 缓冲区

	while (1)  // 循环读文件
	{
		memset(buffer, 0, BUFFER_SIZE); // 清空脏数据
		if (fread(buffer, 1, BUFFER_SIZE, src_fp) == 0)  // 判断是否成功, 成功返回指向缓冲区的指针, 失败或到达文件末尾返回 NULL
		{
			if (ferror(src_fp))  // 判断文件流是否出错
			{
				fclose(dest_fp);
				fclose(src_fp);
				perror("fgets error");
				return -3;
			}
			break;  // 到达文件结尾, 退出循环
		}
		if (fwrite(buffer, 1, BUFFER_SIZE, dest_fp) == 0)
		{
			if (ferror(dest_fp))
			{
				fclose(dest_fp);
				fclose(src_fp);
				perror("fputs error");
				return -4;
			}
		}
	}

	fclose(dest_fp);
	fclose(src_fp);
	return 0;
}
/*
	验证命令:
	diff
	md5sum
*/