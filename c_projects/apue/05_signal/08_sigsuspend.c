/*
	程序演示了 sigsuspend 系统调用的使用
	在信号屏蔽期间打印星号, 然后使用 sigsuspend 原子性地等待信号并恢复处理

	之前的版本：在循环中主动解除阻塞（sigprocmask）来处理信号
	  这个版本：使用 sigsuspend 挂起进程, 等待信号到达后再继续
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/*
	SIGINT SIGRTMIN 信号处理函数
	当收到 SIGINT SIGRTMIN 信号时，向标准输出写入一个 !
*/
void sig_handler(int none)
{
	write(1, "!", 1);  // 往标准输出文件中写一个 !
}

int main(void)
{
	sigset_t old, new;  // old 存储原有的信号集, new 设置需要阻塞的信号集

	sigemptyset(&new);  // 把 new 信号集清空
	sigaddset(&new, SIGINT);  // 将 SIGINT 信号添加到 new 信号集中
	sigaddset(&new, SIGRTMIN);  // 将 SIGRTMIN 信号添加到 new 信号集中

	// 注册信号处理函数
	signal(SIGINT, sig_handler);  // 给 SIGINT 信号设置新行为
	signal(SIGRTMIN, sig_handler);  // 给 SIGRTMIN 信号设置新行为

	// 把 new 中的信号添加到当前屏蔽集中; old 存储原有的屏蔽字
	sigprocmask(SIG_BLOCK, &new, &old);

	// 主循环：共进行 10 轮测试
	for (int i = 0; i < 10; i++)
	{
		// 打印 5 个星号, 每个间隔 1s
		// 在此期间，SIGINT 和 SIGRTMIN 信号被阻塞
		// 用户按 Ctrl+C 或发送 kill -34 不会立即响应
		for (int j = 0; j < 5; j++)
		{
			write(1, "*", 1);
			sleep(1);
		}
		write(1, "\n", 1);  // 换行

		// sigsuspend: 原子性操作
		// 1. 临时将信号屏蔽字设置为 &old (即解除对 SIGINT 和 SIGRTMIN 的阻塞)
		// 2. 挂起进程, 等待信号到来
		// 3. 当信号递送并处理完毕后, 返回前自动恢复原来的信号屏蔽字
		sigsuspend(&old);
	}

	return 0;  // 程序正常结束
}
/*
	为什么用 sigsuspend 而不是用 sigprocmask + pause ?

	因为 sigsuspend 是原子操作, 可以避免竞态条件
	如果先解除阻塞, 再调用 pause, 可能在 pause 之前信号就到达了,
	导致 pause 永远阻塞。sigsuspend 则避免了这个问题
*/
