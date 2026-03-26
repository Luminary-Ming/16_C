#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// 自定义的信号处理函数
static void handler(int none)  // 注册新行为
{
	for (int i = 0; i < 3; i++)
	{
		write(1, "!", 1);  // 往 stdout 中写一个 "!"
		sleep(1);
	}

}

/*
	演示信号处理函数是串行执行的 (证明信号处理不是并发的, 而是串行 (顺序执行) 的)
	在执行信号处理函数期间, 主程序的正常流程会被中断并暂停, 信号处理函数执行完毕后才会继续执行主程序
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

	int i = 0;  // 定义计数器变量, 用于记录打印了多少个星号
	while (1)  // 死循环, 每 1s 往 stdout 中写一个 "*"
	{
		write(1, "*", 1);  // 往 stdout 中写一个 "*"
		sleep(1);  // 休眠 1s

		i++;  // 计数器自增，每打印一个星号就加 1

		if (i == 10)  // 当打印了 10 个星号后 (i 从 0 开始，i==10 表示已经打印了 10 个)		
			// SIGINT  终端中断信号, 通常由 Ctrl+C 产生
			// SIG_DFL 是系统定义的宏, 表示默认的信号处理方式
			signal(SIGINT, SIG_DFL);  // 将 SIGINT 信号的处理方式恢复为默认行为
	}

	return 0;
}
