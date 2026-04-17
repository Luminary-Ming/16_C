/*
	先检查状态, 根据 STATE_R 或 STATE_W 将对应文件描述符加入 fd_set, 仅当 select 返回就绪时才推动
	select 阻塞等待事件 + 条件推动
	当没有 I/O 事件时, 进程处于睡眠状态, 不消耗 CPU
	CPU 占用率极低, 只在有实际数据传输时才工作

	主程序：使用 select 实现事件驱动的状态机调度
	
	核心思路：
	  1. 两个状态机分别负责 tty9 -> tty10 和 tty10 -> tty9 的数据转发
	  2. 每个状态机内部维护自己的状态（读 / 写 / 错误 / 终止）
	  3. 主循环使用 select 监听文件描述符的就绪事件, 仅当描述符可读或可写时才推动对应的状态机
	  4. 避免无意义的忙轮询, 降低 CPU 占用率
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include "01_fsm.h"

#define SELECT  // 启用 select 版本

#define TTY1 "/dev/tty9"
#define TTY2 "/dev/tty10"

int main(void)
{
	fsm_t *fsm12 = NULL;  // 指向 r9w10 的有限状态机
	fsm_t *fsm21 = NULL;  // 指向 r10w9 的有限状态机

#ifdef SELECT
	fd_set rset, wset;  // 定义读集和写集
#endif

	// 打开第一个终端设备 (未显式设置非阻塞, 后续 fsm_init 会统一设置为非阻塞)
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

	fsm_init(&fsm12, fd1, fd2);  // 初始化 r9w10 的有限状态机 (内部会将对应的文件描述符设置为非阻塞)
	fsm_init(&fsm21, fd2, fd1);  // 初始化 r10w9 的有限状态机 (内部会将对应的文件描述符设置为非阻塞)

	// 推动有限状态机的运行, 直到任意一个进入终止状态 (T)
	while (fsm12->state != STATE_T && fsm21->state != STATE_T)
	{
/* ---------- 事件驱动的 select 版本 ---------- */
// 不再无脑推动有限状态机, 而是满足一定条件才推动有限状态机
#ifdef SELECT
		// 如果某个状态机已经进入错误状态 (STATE_E), 立即推动它处理错误 (无需等待 I/O)
		if (fsm12->state == STATE_E)
		{
			fsm_drive(fsm12);  // 推动它处理错误打印错误信息并转到 STATE_T
			continue;  // 跳过本次 select, 继续循环
		}
		if (fsm21->state == STATE_E)
		{
			fsm_drive(fsm21);  // 推动它处理错误打印错误信息并转到 STATE_T
			continue; // 跳过本次 select, 继续循环
		}

		// 清空读集合和写集合, 准备重新设置
		FD_ZERO(&rset);
		FD_ZERO(&wset);

		// 根据 fsm12 的当前状态决定要监听哪个文件描述符
		if (fsm12->state == STATE_R)  // 状态为读: 需要等待读文件描述符可读
			FD_SET(fsm12->rfd, &rset);  // 把读文件描述符加到读集中
		else if (fsm12->state == STATE_W)  // 状态为写: 需要等待写文件描述符可写
			FD_SET(fsm12->wfd, &wset);  // 把写文件描述符加到写集中
		// 根据 fsm21 的当前状态决定要监听哪个文件描述符
		if (fsm21->state == STATE_R)
			FD_SET(fsm21->rfd, &rset);
		else if (fsm21->state == STATE_W)
			FD_SET(fsm21->wfd, &wset);


		// 调用 select 阻塞等待文件描述符就绪 (ready 态)
		// 参数: 最大文件描述符值+1, 读集合, 写集合, 异常集合 (NULL), 超时 (NULL 表示无限等待)
		int maxfd = (fd1 > fd2 ? fd1 : fd2) + 1;
		if (select(maxfd, &rset, &wset, NULL, NULL) == -1)
		{
			if (errno == EINTR)  // 被信号中断, 重新进入循环, 继续 select
				continue;  // 继续下一次循环

			perror("select()");  // 打印错误信息
			goto ERR_1;  // 真正的 select 错误, 跳转到统一清理代码
		}

		// 检查 fsm12 和 fsm21 所关心的文件描述符是否就绪, 若是则推动它
		if(FD_ISSET(fsm12->rfd, &rset) || FD_ISSET(fsm12->wfd, &wset))
			fsm_drive(fsm12);  // 推动 r9w10 的有限状态机
		if (FD_ISSET(fsm21->rfd, &rset) || FD_ISSET(fsm21->wfd, &wset))
			fsm_drive(fsm21);  // 推动 r10w9 的有限状态机
#else
		/* ---------- 无 select 的轮询版本 (仅供对比) ---------- */
		// 无条件推动两个状态机，会导致 CPU 空转 (忙等待)
		fsm_drive(fsm12);  // 推动 r9w10 的有限状态机
		fsm_drive(fsm21);  // 推动 r10w9 的有限状态机
#endif
	}

ERR_1:
	fsm_destroy(fsm12);  // 释放 r9w10 的有限状态机
	fsm_destroy(fsm21);  // 释放 r10w9 的有限状态机

	close(fd1);  // 关闭 /dev/tty9 设备
	close(fd2);  // 关闭 /dev/tty10 设备

	return 0;
}