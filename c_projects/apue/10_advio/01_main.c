/*	
	每次循环无条件调用 fsm_drive(fsm12) 和 fsm_drive(fsm21)
	死循环 + 轮询
	循环会疯狂地反复调用 fsm_drive, 每一次都是无意义的系统调用 (read/write 返回 EAGAIN)
	CPU 占用率飙升 (可能达到 100%), 资源严重浪费
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "01_fsm.h"

#define TTY1 "/dev/tty9"
#define TTY2 "/dev/tty10"

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

	// 推动有限状态机的运行, 直到任意一个进入终止状态 (T)
	while (fsm12->state != STATE_T && fsm21->state != STATE_T)
	{
		fsm_drive(fsm12);  // 推动 r9w10 的有限状态机
		fsm_drive(fsm21);  // 推动 r10w9 的有限状态机
	}

	fsm_destroy(fsm12);  // 释放 r9w10 的有限状态机
	fsm_destroy(fsm21);  // 释放 r10w9 的有限状态机
	
	close(fd1);  // 关闭 /dev/tty9 设备
	close(fd2);  // 关闭 /dev/tty10 设备

	return 0;
}