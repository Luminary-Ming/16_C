/*
	cat 命令实现
	cat 读取文件内容, 打印在 stdout 上
*/
#include <stdio.h>
#include <string.h>  // 使用 memset

#define BUF_SIZE 8192  // 缓冲区大小

int mycat(const char *pathname);

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
	FILE *fp = fopen(pathname, "rb");  // 以读二进制的方式打开文件
	if (fp == NULL)
	{
		fprintf(stderr, "Open File Failed '%s'\n", pathname);
		return -2;
	}

	char buf[BUF_SIZE] = { 0 };  // 定义缓冲区, 存放读取文件的内容
	while (1)  // 循环读文件内容
	{
		// 也不需要使用 memset 清空缓冲区
		//memset(buf, 0, BUF_SIZE);

		// 从文件流中读取数据存到 buf 缓冲区中, 一次读 BUF_SIZE 个数据 (数组大小), 每个数据占 1 字节 (char类型) 
		size_t read_size = fread(buf, 1, BUF_SIZE, fp);  // 获取实际读到的字节数
		if (read_size == 0)  // 如果读到的字节数为 0, 说明到达文件末尾或发生错误 (EOF)
		{
			if (ferror(fp))  // 如果读到了错误
			{
				perror("Read Failed");
				fclose(fp);
				return -3;
			}
			break;  // 否则读完了退出循环
		}

		// 把 buf 缓冲区中的数据写入到 stdout 中, 一次写 read_size 个数据, 每个数据占 1 字节
		fwrite(buf, 1, read_size, stdout);
	}

	fclose(fp);  // 关闭文件流
	return 0;
}

