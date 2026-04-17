#define _GNU_SOURCE  // 启用 GNU 扩展特性
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>

#define BUF_SIZE 128  // 定义缓冲区大小, 用于存储格式化后的时间字符串
#define LOCKFILE "/var/run/mydaemon.pid"  // 定义锁文件的完整路径, 用于实现单实例守护进程

static int mydaemon(void);  // 手动创建守护进程的函数声明
static int process_already_running(void);  // 检查并确保只有一个实例运行的函数声明

int main(int argc, char *argv[])
{
	int ret = 0;  // 返回码, 0 表示成功, 非 0 表示错误类型

	openlog(argv[0], LOG_PID | LOG_PERROR, LOG_DAEMON);
	// 与系统日志文件

#if 0
	if (mydaemon() < 0)  // 手动创建守护进程的方式
	{
		fprintf(stderr, "Mydaemon Failed!\n");  // 打印错误
		goto ERR_1;
	}

#else
	// 使用系统提供的 daemon 函数创建守护进程
	if (daemon(0, 1) == -1)  // 参数 0 切换工作目录到根目录; 参数 1 不重定向标准输入输出到 /dev/null
	{
		//perror("daemon()");
		syslog(LOG_ERR, "daemon() : %s", strerror(errno));
		goto ERR_1;
	}

#endif
	if (process_already_running() < 0)  // 检查进程是否已经在运行, 调用函数尝试获取文件锁
	{
		//fprintf(stderr, "process_already_running() is failed\n");  // 如果获取锁失败，打印错误信息
		syslog(LOG_ERR, "process_already_running() is failed");
		goto ERR_1;
	}

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
	const int MAX_COUNT = 100;  // 最大写入次数, 避免无限循环

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
	pid_t pid = fork();  // 创建子进程
	if (pid < 0)
	{
		perror("fork()");
		return -1;
	}

	if (pid > 0)  // 父进程
		_exit(0);  // 终止父进程 (不需要终止处理程序)

	if (setsid() == (pid_t)-1)  // 创建新会话，使子进程成为新会话的会长和新进程组的组长 (失败时返回 -1)
	{
		perror("setsid()");
		return -2;
	}

	// 执行到此, 当前的子进程就新的会话 新的进程组的组长进程
	// 此时, 子进程 PID == PGID == SID 并且脱离了控制终端

	// 文件屏蔽字要设置为 0
	umask(0);  // 清除文件权限掩码, 确保守护进程创建的文件具有指定的权限

	// 当前工作路径切换到根目录, 避免占用可卸载的文件系统
	if (chdir("/") == -1)
	{
		perror("chdir()");
		return -3;
	}

	// 将文件描述符 0 1 2 重定向到 "/dev/null" (防止守护进程误向终端输出信息或从终端读取输入)
	int fd = open("/dev/null", O_RDWR);  // 以读写的形式打开 /dev/null 文件
	if (fd < 0)
	{
		perror("open()");
		return -4;
	}

	dup2(fd, 0);  // 将文件描述符 0 重定向到 fd 指向的 /dev/null
	dup2(fd, 1);  // 将文件描述符 1 重定向到 fd 指向的 /dev/null
	dup2(fd, 2);  // 将文件描述符 2 重定向到 fd 指向的 /dev/null
	if (fd > 2)   // 关闭多余的 fd 文件描述符
		close(fd);

	return 0;
}

// 检查进程是否已经在运行 (实现单实例守护进程)
// 打开或创建一个锁文件
// 对文件加互斥锁 (非阻塞模式)
// 如果加锁成功, 将当前进程的 PID 写入文件
// 如果加锁失败, 说明已有实例在运行
static int process_already_running(void)
{
	char buf[BUF_SIZE];  // 声明缓冲区用于存储 PID 字符串

	umask(0); // 文件屏蔽字设置为 0, 确保创建的文件具有指定的完整权限

	int fd = open(LOCKFILE, O_WRONLY | O_CREAT, 0666);  // 以只写形式打开或者创建互斥锁文件
	if (fd < 0)
	{
		perror("open()");
		return -1;
	}

	// 尝试给文件加互斥锁（非阻塞模式）
	// LOCK_EX 互斥锁 (同一时间只能有一个进程持有)  
	// LOCK_NB 非阻塞模式 (如果无法获取锁立即返回错误, 不等待)
	if (flock(fd, LOCK_EX | LOCK_NB) == -1)
	{
		perror("flock()");
		close(fd);
		return -2;
	}

	// 获取锁成功后, 将文件内容截断为 0
	// 这样可以清除文件中可能残留的旧 PID 信息
	if (ftruncate(fd, 0) == -1)
	{
		perror("ftruncate()");
		close(fd);
		return -3;
	}

	// 将当前进程的 PID 格式化为字符串并写入锁文件
	sprintf(buf, "%d\n", getpid());
	write(fd, buf, strlen(buf)) != strlen(buf); // 获取的锁文件进程的 PID 字符串写入锁文件中

	return 0;  // 返回 0 表示成功, 当前是唯一运行的实例
}
/*
	neovide 查看下 : 
		向前搜索  /a\.out (\. 转义)
		向后搜索  ?a\.out
	

	tail -f /tmp/out  // 默认显示文件的最后 10 行内容, 然后实时显示新增内容
*/