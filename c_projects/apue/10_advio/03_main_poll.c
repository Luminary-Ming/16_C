/*
 * 程序功能：使用有限状态机 (FSM) 实现两个终端设备之间的双向数据传输
 *
 * 核心机制:
 * 1. 创建两个有限状态机: fsm12 (从 tty9 读数据写到 tty10) 和 fsm21 (从 tty10 读数据写到 tty9)
 * 2. 使用 poll() 系统调用进行 I/O 事件监控, 避免忙等待 (busy loop)
 * 3. 只有文件描述符就绪时才驱动对应的状态机, 大幅降低 CPU 占用率
 *
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include "01_fsm.h"
#include <sys/select.h>

#define POLL  // 启用 poll() 版本

#define TTY1 "/dev/tty9"
#define TTY2 "/dev/tty10"

int main(void)
{
	fsm_t *fsm12 = NULL;  // 指向 r9w10 的有限状态机
	fsm_t *fsm21 = NULL;  // 指向 r10w9 的有限状态机

#ifdef POLL
	// poll 文件描述符数组, 用于监控两个终端设备
	struct pollfd pdf[2];  // pdf[0] 监控 tty9; pdf[1] 监控 tty10
#endif

	int fd1 = open(TTY1, O_RDWR);  // 打开 /dev/tty9 设备 (打开时没有加阻塞)
	if (fd1 == -1)
	{
		perror("open()");
		return -1;
	}
	write(fd1, "[*****tty9*****]", 16);  // 用来区分是 /dev/tty9 设备

	int fd2 = open(TTY2, O_RDWR | O_NONBLOCK);  // 打开 /dev/tty10 设备 (打开时没有加阻塞)
	if (fd2 == -1)
	{
		perror("open()");
		close(fd1);  // 关闭 /dev/tty9 设备
		return -2;
	}
	write(fd2, "[!!!!!tty10!!!!!]", 17);  // 用来区分是 /dev/tty10 设备

	fsm_init(&fsm12, fd1, fd2);  // 初始化 r9w10 的有限状态机
	fsm_init(&fsm21, fd2, fd1);  // 初始化 r10w9 的有限状态机

#ifdef POLL
	// 配置 poll 监控的文件描述符
	pdf[0].fd = fd1;  // pdf[0] 监控 /dev/tty9 文件描述符
	pdf[1].fd = fd2;  // pdf[1] 监控 /dev/tty10 文件描述符
#endif

	// 推动有限状态机的运行, 直到任意一个进入终止状态 (T)
	while (fsm12->state != STATE_T && fsm21->state != STATE_T)
	{
#ifdef POLL
		// 清空 poll(2) 中监听的事件
		pdf[0].events = 0;  // 清空 pdf[0] 监听的事件
		pdf[1].events = 0;  // 清空 pdf[1] 监听的事件

		// 根据 fsm12 的当前状态决定要监控哪些 I/O 事件
		if (fsm12->state == STATE_R)
			pdf[0].events |= POLLIN;  // 监控 tty9 是否可读 (有输入数据)
		else if (fsm12->state == STATE_W)
			pdf[1].events |= POLLOUT;  // 监控 tty10 是否可写 (缓冲区空闲)

		if (fsm21->state == STATE_R)
			pdf[1].events |= POLLIN;  // 监控 tty10 是否可读 (有输入数据)
		else if (fsm21->state == STATE_W)
			pdf[0].events |= POLLOUT;  // 监控 tty9 是否可写 (缓冲区空闲)


		// 调用 poll() 等待 I/O 事件发生 (阻塞直到有事件或信号中断)
		// 参数: 监控的文件描述符数组; 数组大小; 超时时间 -1 表示无限等待
		if (poll(pdf, 2, -1) == -1)
		{
			if (errno == EINTR)  // 被信号中断 (如 Ctrl+C)
				continue;  // 跳过本次循环, 重新调用 poll
			perror("poll()");  // 打印错误信息
			goto ERR_1;  // 跳转到错误处理标签
		}

		// 根据 poll() 返回的事件驱动对应的状态机
		// fsm12 被驱动的条件: tty9 可读 或 tty10 可写
		// 条件1: tty9 可读 (有输入数据) OR tty10 可写 (缓冲区空闲)
		// 满足任一条件, 说明 fsm12 可以执行 I/O 操作
		if (pdf[0].revents & POLLIN || pdf[1].revents & POLLOUT)
			fsm_drive(fsm12);  // 推动 r9w10 的有限状态机

		// 条件2: tty10 可读 (有输入数据) OR tty9 可写 (缓冲区空闲)
		// fsm21 被驱动的条件: tty10 可读 或 tty9 可写
		// 满足任一条件, 说明 fsm21 可以执行 I/O 操作
		if (pdf[1].revents & POLLIN || pdf[0].revents & POLLOUT)
			fsm_drive(fsm21);  // 推动 r10w9 的有限状态机

#else
		// ----- 无 poll 的忙等待版本（仅供对比，生产环境不建议使用）-----
		// 每次循环无条件调用 fsm_drive，不管文件描述符是否就绪
		// 缺点：
		// 1. 频繁调用 read()/write() 系统调用，返回 EAGAIN（资源暂时不可用）
		// 2. CPU 占用率飙升到 100%，严重浪费系统资源
		// 3. 效率极低，不适合实际生产环境
		fsm_drive(fsm12);  // 推动 r9w10 的有限状态机
		fsm_drive(fsm21);  // 推动 r10w9 的有限状态机
#endif
		
	}

ERR_1:  // 清理资源
	fsm_destroy(fsm12);  // 释放 r9w10 的有限状态机
	fsm_destroy(fsm21);  // 释放 r10w9 的有限状态机

	close(fd1);  // 关闭 /dev/tty9 设备
	close(fd2);  // 关闭 /dev/tty10 设备

	return 0;
}