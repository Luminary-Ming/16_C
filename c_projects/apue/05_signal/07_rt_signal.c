/*
	演示信号屏蔽（阻塞）机制
	展示如何临时阻塞 SIGINT 信号 (Ctrl+C), 并在特定时间段后恢复信号的处理

	演示信号屏蔽：在打印星号期间阻塞 SIGINT 信号
	演示信号恢复：在星号打印完毕后恢复接收 SIGINT
	验证信号挂起：如果用户在阻塞期间按 Ctrl+C，信号会被暂存，待解除阻塞后统一处理

	运行效果
	*****!*****!*****!...
	打印 5 个星号期间，按 Ctrl+C 不会立即响应
	打印完 5 个星号后，如果有未处理的 SIGINT，会输出一个 !
	然后继续下一轮 5 个星号的打印
	如果连续用 kill -34 pid 发送 SIGRTMIN 信号就会 *!*!*!*!*!.... 这样打印
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/*
	SIGINT 信号处理函数
	当收到 SIGINT 信号时，向标准输出写入一个 !
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

	signal(SIGINT, sig_handler);  // 给 SIGINT 信号设置新行为
	signal(SIGRTMIN, sig_handler);  // 给 SIGRTMIN 信号设置新行为

	// 主循环：共进行 10 轮测试
	for (int i = 0; i < 10; i++)
	{
		// 阻塞 SIGINT 信号
		// SIG_BLOCK 表示将 new 中的信号添加到当前信号屏蔽字中
		// &old 保存原有的信号屏蔽字, 以便后续恢复
		sigprocmask(SIG_BLOCK, &new, &old);

		// 打印 5 个星号, 每个间隔 1s
		// 在此期间, SIGINT 信号被阻塞, 按 Ctrl+C 不会立即响应
		for (int j = 0; j < 5; j++)
		{
			write(1, "*", 1);
			sleep(1);
		}
		write(1, "\n", 1);  // 换行

		// 恢复原有的信号屏蔽字 (解除对 SIGINT 的阻塞)
		// SIG_SETMASK 表示将当前信号屏蔽字设置为 old
		sigprocmask(SIG_SETMASK, &old, NULL);
	}

	return 0;  // 程序正常结束
}

