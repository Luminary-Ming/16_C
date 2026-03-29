#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// SIGALRM 信号处理函数
void sig_handler(int none)
{
	write(1, "!", 1);
}

/*
	演示了 alarm() 函数的覆盖特性
	多次调用 alarm() 时, 只有最后一次设置生效, 之前的闹钟会被取消
*/
int main(void)
{
	struct sigaction act; // 用于配置信号处理方式

	act.sa_handler = sig_handler;  // 指定信号处理函数为 sig_handler
	sigemptyset(&act.sa_mask);  // 清空信号掩码集, 处理信号时不阻塞其他信号
	act.sa_flags = 0;  // 不使用特殊标志 

	// 将 SIGALRM 信号与 act 配置绑定
	sigaction(SIGALRM, &act, NULL);

	// alarm() 函数特性演示
	// alarm() 设置一个定时器, 在指定秒数后向进程发送 SIGALRM 信号
	// 每个进程只能有一个活动的 alarm 定时器
	// 后续调用会覆盖前一个定时器
	alarm(1);  // 此时定时器设置为 1 秒后触发
	alarm(10);  // 此时定时器被重置为 10 秒后触发, 之前的 1 秒定时器被取消
	alarm(5);  // 最终只有这个定时器生效，5 秒后会触发一次 SIGALRM 信号

	// 每秒输出一个星号, 用于观察 alarm 定时器的触发时间
	while (1)
	{
		write(1, "*", 1);
		sleep(1);
	}
}
