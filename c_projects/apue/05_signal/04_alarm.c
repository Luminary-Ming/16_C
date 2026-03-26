#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>

void sig_handler(int none)
{
	alarm(1);  // 形成了 alarm 链 (可以周期性的产生 SIGALRM 信号了)
	write(1, "!", 1);
} 

int main(void)
{
	signal(SIGALRM, sig_handler);  // 给 SIGALRM 信号设置新行为
	alarm(1);  // 设置 1s 闹钟后首次触发 SIGALRM

	while (1);  // 死循环

	return 0;
}
/*
	内核发送 SIGALRM 信号

	进程暂停当前工作, 执行 sig_handler 函数

	在 sig_handler 中 :

	alarm(1) → 重新设置 1s 后的闹钟

	write(1, "!", 1) → 输出一个 "!"

	返回主程序的 while(1) 死循环

	1s 时间到了内核再次发送 SIGALRM 信号

	进程暂停当前工作, 执行 sig_handler 函数

	在 sig_handler 中 :

	alarm(1) → 重新设置 1s 后的闹钟

	write(1, "!", 1) → 输出一个 "!"

	循环此过程...
*/