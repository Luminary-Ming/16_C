/*
	cat 命令实现
	cat 读取文件内容, 打印在 stdout 上
*/
#include <stdio.h>

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
	FILE *fp = fopen(pathname, "r");  // 以读的方式打开文件
	if (fp == NULL)
	{
		fprintf(stderr, "Open File Failed '%s'\n", pathname);
		return -2;
	}

	int ch = 0;
	while (1)  // 循环读文件
	{
		ch = fgetc(fp);  // 从文件流中读取一个字符
		if (ch == EOF)  // 如果读到了 EOF (宏值 -1, 读到了文件末尾或读到了错误)
		{
			if (ferror(fp))  // 如果读到了错误
			{
				perror("Read Failed");
				fclose(fp);
				return -3;
			}
			break;  // 否则读到了文件末尾, 跳出循环
		}
		fputc(ch, stdout);  // 把每一个读取到的字符打印在 stdout 上
	}

	fclose(fp);  // 关闭文件流
	return 0;
}

