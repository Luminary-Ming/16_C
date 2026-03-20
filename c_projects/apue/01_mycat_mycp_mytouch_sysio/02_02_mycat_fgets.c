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
	FILE *fp = fopen(pathname, "r");  // 以读的方式打开文件
	if (fp == NULL)
	{
		fprintf(stderr, "Open File Failed '%s'\n", pathname);
		return -2;
	}

	char buf[BUF_SIZE] = {0};  // 定义缓冲区, 存放读取文件的内容
	while (1)  // 循环读文件内容
	{
		// 不需要使用 memset 清空缓冲区, fgets() 会在读取字符串末尾自动添加 '\0', 下一次调用 fgets() 会覆盖缓冲区中的内容
		//memset(buf, 0, BUF_SIZE);

		// fgets() 成功返回指向缓冲区的指针, 失败返回 NULL (读到了 EOF)
		if (fgets(buf, BUF_SIZE, fp) == NULL)  // 判断是否读取完数据
		{
			if (ferror(fp))  // 如果读到了错误
			{
				perror("Read Failed");
				fclose(fp);
				return -3;
			}
			break;  // 否则读完了退出循环
		}

		fputs(buf, stdout);  // 将缓冲区中读取的内容打印到 stdout 上
	}

	fclose(fp);  // 关闭文件流
	return 0;
}

