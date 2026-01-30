/*
	cat 命令实现
*/
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc < 2)  // 判断命令行参数的个数是否少于2个
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

	int ch = 0;
	while (1)  // 循环读文件
	{
		ch = fgetc(fp);
		if (ch == EOF)  // 判断是否读到了 EOF(文件末尾或者错误)
		{
			if (ferror(fp))  // 判断文件流是否出错
			{
				fclose(fp);  // 关闭文件流
				return -2;
			}
			break;
		}
		fputc(ch, stdout); //把 ch 存储的字符数据写入到 stdout 标准输出流中
	}

	fclose(fp);  // 关闭文件流
	return 0;
}