#define _GNU_SOURCE  // 启用 GNU 扩展特性
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define BUF_SIZE 128  // 定义缓冲区大小, 用于存储格式化后的时间字符串

int main(void)
{
	int ret = 0;  // 返回码, 0 表示成功, 非 0 表示错误类型

	FILE *fp = fopen("/tmp/out", "w");  // 以写模式打开文件 /tmp/out
	if (fp == NULL)
	{
		perror("fopen()");
		ret = -1;
		goto ERR_1;
	}

	time_t tm;  // 存储时间戳 (从 1970-01-01 开始的秒数)
	struct tm now;  // 存储分解后的本地时间
	char buf[BUF_SIZE];  // 存储格式化后的时间字符串
	int count = 0;  // 循环计数器, 控制写入次数
	const int MAX_COUNT = 10;  // 最大写入次数, 避免无限循环

	// 循环写入时间信息到文件
	while (count < MAX_COUNT)
	{
		// 获取时间戳 (失败时返回 (time_t)-1)
		if (time(&tm) == (time_t)-1)
		{
			perror("time()");
			ret = -2;
			goto ERR_2;
		}

		// 将时间戳转换为本地时间
		if (localtime_r(&tm, &now) == NULL)
		{
			perror("localtime_r()");
			ret = -3;
			goto ERR_2;
		}
		
		// 将本地时间格式化为指定格式的字符串
		strftime(buf, BUF_SIZE, "%Y-%m-%d %H:%M:%S\n", &now);

		// 将格式化后的时间字符串写入文件
		fputs(buf, fp);  // 把 buf 存储的字符串写入 fp 指向的文件流中
		fflush(fp);  // 刷新文件缓冲区
		sleep(1);  // 每 1s 写一次, 让时间记录有间隔

		count++;  // 循环计数器加1, 控制写入次数
	}

ERR_2:
	fclose(fp);  // 关闭文件流

ERR_1:
	return ret;
}
/*
	2026-03-25 13:21:31
	2026-03-25 13:21:32
	2026-03-25 13:21:33
	2026-03-25 13:21:34
	2026-03-25 13:21:35
	...
*/
