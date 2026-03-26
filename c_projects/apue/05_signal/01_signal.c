#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// 自定义的信号处理函数
static void handler(int none)  // 注册新行为
{
	write(1, "!", 1);  // 往 stdout 中写一个 "!"
}

/*
	演示信号处理机制, 当按下 Ctrl+C 时, 程序会打印一个 "!" 而不是终止程序
*/
int main(void)
{
	// SIGINT  终端中断信号, 通常由 Ctrl+C 产生
	// handler 自定义的信号处理函数
	// 返回之前的信号处理函数指针
	sighandler_t old = signal(SIGINT, handler);  // 给 SIGINT 信号设置新行为
	if (old == SIG_ERR)  // 检查信号处理函数设置是否成功 (SIG_ERR 是 signal() 返回的错误标志)
	{
		perror("signal()");
		return -1;
	}

	while (1)  // 死循环, 每 1s 往 stdout 中写一个 "*"
	{
		write(1, "*", 1);  // 往 stdout 中写一个 "*"
		sleep(1);  // 休眠 1s
	}

	return 0;
}
