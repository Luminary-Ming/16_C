#define _GNU_SOURCE  // 启用 GNU 扩展特性
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define BUF_SIZE 128  // 定义缓冲区大小, 用于存储格式化后的时间字符串

static int mydaemon(void);

int main(void)
{
	int ret = 0;  // 返回码, 0 表示成功, 非 0 表示错误类型
	FILE *fp = NULL;
	int count = 0;  // 循环计数器, 控制写入次数
	const int MAX_COUNT = 10;  // 最大写入次数, 避免无限循环

#if 0
	if (mydaemon() < 0)
	{
		fprintf(stderr, "Mydaemon Failed!\n");  // 打印错误
		goto ERR_1; 
	}

#else
	if (daemon(0, 0) == -1)  // 创建守护进程
	{
		perror("daemon()");
		goto ERR_1;
	}

#endif

	fp = fopen("/tmp/out", "w");  // 以写模式打开文件 /tmp/out
	if (fp == NULL)
	{
		perror("fopen()");
		ret = -1;
		goto ERR_1;
	}

	time_t tm;  // 存储时间戳 (从 1970-01-01 开始的秒数)
	struct tm now;  // 存储分解后的本地时间
	char buf[BUF_SIZE];  // 存储格式化后的时间字符串

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

static int mydaemon(void)
{
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork()");
		return -1;
	}

	if (pid > 0)  // 父进程
		_exit(0);  // 终止父进程 (不需要终止处理程序)

	if (setsid() == (pid_t)-1)  // 创建一个新会话 (失败时返回 -1)
	{
		perror("setsid()");
		return -2;
	}

	// 执行到此, 当前的子进程就新的会话 新的进程组的组长进程
	// 此时, 子进程 PID == PGID == SID 而且脱离了控制终端
	// 往下 

	// 文件屏蔽字要设置为 0
	umask(0);

	// 当前工作路径切换到根目录
	if (chdir("/") == -1)
	{
		perror("chdir()");
		return -3;
	}

	// 将文件描述符 0 1 2 重定向到 "/dev/null"
	int fd = open("/dev/null", O_RDWR);  // 以读写的形式打开 /dev/null 文件
	if (fd < 0)
	{
		perror("open()");
		return -4;
	}

	dup2(fd, 0);  // 将文件描述符 0 重定向到 fd 文件
	dup2(fd, 1);  // 将文件描述符 1 重定向到 fd 文件
	dup2(fd, 2);  // 将文件描述符 2 重定向到 fd 文件
	if (fd > 2)
		close(fd);

	return 0;
}
/*
	./a.out 已经脱离终端, 变成守护进程在后台运行
	
	xinming@03_daemon$ gcc 04_write_time_into_file_setsid.c 
	xinming@03_daemon$ ./a.out 
	xinming@03_daemon$ 

	xinming@03_daemon$ ps -ajx | grep './a.out'
	1676    6556    6556    6556 ?             -1 Ss    1000   0:00 ./a.out
	3414    6558    6557    3414 pts/0       6557 S+    1000   0:00 grep --color=auto ./a.out
	xinming@03_daemon$

	xinming@~$ cat / tmp / out
	2026 - 03 - 25 15:02 : 09
	2026 - 03 - 25 15:02 : 10
	2026 - 03 - 25 15 : 02 : 11
	2026 - 03 - 25 15 : 02 : 12
	2026 - 03 - 25 15 : 02 : 13
	2026 - 03 - 25 15 : 02 : 14
	2026 - 03 - 25 15 : 02 : 15
	2026 - 03 - 25 15 : 02 : 16
	2026 - 03 - 25 15 : 02 : 17
	2026 - 03 - 25 15 : 02 : 18
	xinming@~$
*/
