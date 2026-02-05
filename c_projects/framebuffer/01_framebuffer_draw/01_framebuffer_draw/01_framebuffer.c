/*
	framebuffer 帧缓冲
	使用系统调用 IO 的方式驱动 framebuffer 设备
*/
#include <stdio.h>
#include <sys/types.h>  // man 2 open
#include <sys/stat.h>   // man 2 open
#include <fcntl.h>      // man 2 open
#include <unistd.h>     // man 2 close
#include <sys/ioctl.h>  // man 2 ioctl
#include <linux/fb.h>   // 有关 framebuffer 的头文件
#include <sys/mman.h>   // man 2 mmap

int main(void)
{
	int fb = open("/dev/fb0", O_RDWR);   // 以读写的形式打开 framebuffer 设备
	if (fb < 0)  // 判断打开 framebuffer 设备是否失败
	{
		perror("open()");
		return -1;
	}

	// 打印新的文件描述符
	printf("fb = %d\n", fb);  // 输出: fb = 3

	struct fb_var_screeninfo var;  // 可变屏幕信息的结构体变量

	int ret = ioctl(fb, FBIOGET_VSCREENINFO, &var);  // 获取可变屏幕的信息
	if (ret < 0)
	{
		perror("ioctl()");
		close(fb);
		return -2;
	}

	// 分辨率
	printf("%d * %d\n", var.xres, var.yres);  // 1280 * 800
	// 每个像素的位数
	printf("%d bits color\n", var.bits_per_pixel);  // 32 bits color

	close(fb);  // 关闭 framebuffer 设备
	
	return 0;
}
/*
	printf("fb = %d\n", fb);  // 输出: fb = 3
	在 Linux 中，每个进程启动时都会自动打开三个标准文件描述符：
	0   stdin     标准输入（键盘）
	1   stdout    标准输出（屏幕）
	2   stderr    标准错误（屏幕）

	0  1  2 已经被标准输入/输出/错误占用, 所以是 3
*/