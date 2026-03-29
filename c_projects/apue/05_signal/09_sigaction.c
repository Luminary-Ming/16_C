/*
	程序演示了 sigsuspend 系统调用的使用
	在信号屏蔽期间打印星号, 然后使用 sigsuspend 原子性地等待信号并恢复处理

	之前的版本：在循环中主动解除阻塞（sigprocmask）来处理信号
	  这个版本：使用 sigsuspend 挂起进程, 等待信号到达后再继续

	修改为 sigaction(2) 的版本
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
	// oldact 保存旧的信号处理方式
	// newact 设置新的信号处理方式
	struct sigaction oldact, newact;

	oldact.sa_handler = SIG_DFL;  // 设置处理函数为默认行为 (0)
	oldact.sa_flags = 0;  // 不设置特殊标志
	sigemptyset(&oldact.sa_mask);  // 清空信号集
	
	newact.sa_handler = sig_handler;  // 设置处理函数为 sig_handler
	newact.sa_flags = 0;  // 不设置特殊标志
	sigemptyset(&newact.sa_mask);  // 清空信号集
	
	// 设置信号处理函数执行期间需要额外阻塞的信号
	sigaddset(&newact.sa_mask, SIGINT);
	sigaddset(&newact.sa_mask, SIGRTMIN);

	// 设置信号处理方式
	sigaction(SIGINT, &newact, NULL);  // 给 SIGINT 信号设置新的行为
	sigaction(SIGRTMIN, &newact, NULL);  // 给 SIGRTMIN 信号设置新的行为

	// 把 new 中的信号添加到当前屏蔽集中; old 存储原有的屏蔽字
	sigprocmask(SIG_BLOCK, &newact.sa_mask, &oldact.sa_mask);

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
		// 1. 临时将信号屏蔽字设置为 &oldact.sa_mask (即解除对 SIGINT 和 SIGRTMIN 的阻塞)
		// 2. 挂起进程, 等待信号到来
		// 3. 当信号递送并处理完毕后, 返回前自动恢复原来的信号屏蔽字
		sigsuspend(&oldact.sa_mask);
	}

	return 0;  // 程序正常结束
}
/*
	为什么用 sigsuspend 而不是用 sigprocmask + pause ?

	因为 sigsuspend 是原子操作, 可以避免竞态条件
	如果先解除阻塞, 再调用 pause, 可能在 pause 之前信号就到达了,
	导致 pause 永远阻塞。sigsuspend 则避免了这个问题
*/