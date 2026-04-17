/*
 * 程序功能：使用有限状态机 (FSM) 实现两个终端设备之间的双向数据传输
 *
 * 核心机制:
 * 1. 创建两个有限状态机: fsm12 (从 tty9 读数据写到 tty10) 和 fsm21 (从 tty10 读数据写到 tty9)
 * 2. 使用 epoll() 系统调用族进行 I/O 事件监控, 避免忙等待 (busy loop)
 * 3. 只有文件描述符就绪时才驱动对应的状态机, 大幅降低 CPU 占用率
 * 4. epoll 版本效率最高, 但仅适用于 Linux 系统
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include "01_fsm.h"

#define EPOLL  // 启用 epoll() 版本

#define TTY1 "/dev/tty9"
#define TTY2 "/dev/tty10"

// 定义自定义数据结构, 用于在 epoll_event 中传递状态机指针
typedef struct
{
	fsm_t *fsm;  // 指向状态机的指针
	int fd;  // 文件描述符
	int event_type;  // 事件类型
}epoll_data_custom_t;

int main(void)
{
	fsm_t *fsm12 = NULL;  // 指向 r9w10 的有限状态机
	fsm_t *fsm21 = NULL;  // 指向 r10w9 的有限状态机

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

#ifdef EPOLL
	int epfd = epoll_create(1);  // 创建 epoll 实例, 参数 size 在 Linux 2.6.8 之后已忽略, 但必须 > 0
	if (epfd == -1)
	{
		perror("epoll_create()");
		goto ERR_1;
	}

	epoll_data_custom_t data12;  // fsm12 的自定义数据
	data12.fsm = fsm12;
	data12.fd = fd1;

	epoll_data_custom_t data21;  // fsm21 的自定义数据
	data21.fsm = fsm21;
	data21.fd = fd2;
#endif

	// 推动有限状态机的运行, 直到任意一个进入终止状态 (T)
	while (fsm12->state != STATE_T && fsm21->state != STATE_T)
	{
#ifdef EPOLL
		// 第一步: 根据当前的状态机的状态, 动态调整 epoll 监听的事件

		// 删除之前为 fd1 注册的所有事件
		struct epoll_event ev;  // 注册事件的结构体

		// 根据 fsm12 的当前状态, 为相应的文件描述符注册事件
		if (fsm12->state == STATE_R)
		{
			// fsm12 需要从 fd1 (tty9) 读取数据
			ev.events = EPOLLIN;  // 监听可读事件
			ev.data.ptr = &data12;
			if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &ev) == -1)
			{
				if (errno != EEXIST)  // EEXIST 表示已存在, 忽略
				{
					perror("epoll_ctl ADD fd1 for fsm12 read");
					goto ERR_1;
				}
			}
		}
		else if (fsm12->state == STATE_W)
		{
			// fsm12 需要向 fd2 (tty10) 写入数据
			ev.events = EPOLLOUT;  // 监听可写事件
			ev.data.ptr = &data12;
			if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &ev) == -1)
			{
				if (errno != EEXIST)
				{
					perror("epoll_ctl ADD fd2 for fsm12 write");
					goto ERR_1;
				}
			}
		}
		else
		{
			// fsm12 处于其他状态 (E 或 T), 删除其注册的事件
			ev.events = 0;
			ev.data.ptr = &data12;
			epoll_ctl(epfd, EPOLL_CTL_DEL, fd1, &ev);
			epoll_ctl(epfd, EPOLL_CTL_DEL, fd2, &ev);
		}

		// 根据 fsm21 的当前状态, 为相应的文件描述符注册事件
		if (fsm21->state == STATE_R)
		{
			// fsm21 需要从 fd2 (tty10) 读取数据
			ev.events = EPOLLIN;  // 监听可读事件
			ev.data.ptr = &data21;
			if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &ev) == -1)
			{
				if (errno != EEXIST)
				{
					perror("epoll_ctl ADD fd2 for fsm21 read");
					goto ERR_1;
				}
			}
		}
		else if (fsm21->state == STATE_W)
		{
			// fsm21 需要向 fd1 (tty9) 写入数据
			ev.events = EPOLLOUT;  // 监听可写事件
			ev.data.ptr = &data21;
			if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &ev) == -1)
			{
				if (errno != EEXIST)
				{
					perror("epoll_ctl ADD fd1 for fsm21 write");
					goto ERR_1;
				}
			}
		}
		else
		{
			// fsm21 处于其他状态 (E 或 T), 删除其注册的事件
			ev.events = 0;
			ev.data.ptr = &data21;
			epoll_ctl(epfd, EPOLL_CTL_DEL, fd2, &ev);
			epoll_ctl(epfd, EPOLL_CTL_DEL, fd1, &ev);
		}

		// 第二步：调用 epoll_wait 等待事件就绪
		struct epoll_event events[2]; // 用于存储就绪事件的数组

		// epfd - epoll实例;
		// events - 存储就绪事件的数组;
		// maxevents - 数组大小;
		// timeout - 超时时间 (-1表示无限等待)
		int nfds = epoll_wait(epfd, events, 2, -1);
		if (nfds == -1)
		{
			if (errno == EINTR)  // 被信号中断 (Ctrl+C)
				continue;
			perror("epoll_wait()");
			goto ERR_1;
		}

		// 第三步：根据就绪的事件驱动对应的状态机
		for (int i = 0; i < nfds; i++)
		{
			epoll_data_custom_t *data = (epoll_data_custom_t *)events[i].data.ptr;
			fsm_t *fsm = data->fsm;

			// 驱动状态机
			fsm_drive(fsm);
		}
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
	if (epfd > 0) close(epfd);  // 关闭 epoll 实例

	fsm_destroy(fsm12);  // 释放 r9w10 的有限状态机
	fsm_destroy(fsm21);  // 释放 r10w9 的有限状态机

	close(fd1);  // 关闭 /dev/tty9 设备
	close(fd2);  // 关闭 /dev/tty10 设备

	return 0;
}