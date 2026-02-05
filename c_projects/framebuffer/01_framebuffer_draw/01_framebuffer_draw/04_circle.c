/*
	 画一个圆心 (200, 200) 半径为 100 的实心圆
*/
#include <stdio.h>
#include <sys/types.h>  // man 2 open
#include <sys/stat.h>   // man 2 open
#include <fcntl.h>      // man 2 open
#include <unistd.h>     // man 2 close
#include <sys/ioctl.h>  // man 2 ioctl
#include <linux/fb.h>   // 有关 framebuffer 的头文件
#include <sys/mman.h>   // man 2 mmap
#include <stdlib.h>     // srand 函数

/*
	功能 : 绘制单个像素点
	   x, y  像素点的坐标
	  color  颜色值
		 pc  帧缓冲内存指针
line_length  每行像素所占的字节数
	  bytes  单个像素点所占的字节数
*/
void draw_point(int x, int y, int color,
				unsigned char *pc,
				size_t line_length,
				size_t bytes);

/*
	功能 : 绘制圆
	   x, y  圆心坐标
	 radius  半径
	  color  颜色值
		 pc  帧缓冲内存指针
line_length  每行像素所占的字节数
	  bytes  单个像素点所占的字节数
*/
void draw_circle(int x, int y,
				 int radius, int color,
				 unsigned char *pc,
				 size_t line_length,
				 size_t bytes);

int main(void)
{
	srand(getpid());  // 使用进程 ID 作为随机数种子

	int fb = open("/dev/fb0", O_RDWR);   // 以读写的形式打开 framebuffer 设备
	if (fb < 0)  // 判断打开 framebuffer 设备是否失败
	{
		perror("open()");
		return -1;
	}

	struct fb_var_screeninfo var;  // 可变屏幕信息的结构体变量
	int ret = ioctl(fb, FBIOGET_VSCREENINFO, &var);  // 获取可变屏幕的信息
	if (ret < 0)
	{
		perror("ioctl()");
		close(fb);
		return -2;
	}

	struct fb_fix_screeninfo fix;  // 固定屏幕信息的结构体变量
	ret = ioctl(fb, FBIOGET_FSCREENINFO, &fix);  // 执行 ioctl 获取信息
	if (ret < 0)
	{
		perror("ioctl()");
		close(fb);
		return -3;
	}


	// fix.smem_len 帧缓冲设备的总内存大小 ( 总字节大小 )
	// MAP_SHARED 创建共享的内存映射
	unsigned char *pc = (unsigned char *)mmap(0, fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);  // 把 framebuffer 映射到内存中
	if (pc == (void *)(-1))  // 判断把 framebuffer 映射到内存中是否失败
	{
		perror("mmap()");
		close(fb);
		return -3;
	}

	size_t bytes = var.bits_per_pixel >> 3;  // 单个像素所占字节数

	int x = 200, y = 200;  // 圆心
	int radius = 100;  // 半径

	while (1)  // 持续绘制随机颜色的矩形
	{
		int color = rand() % 0xFFFFFF;  // 随机颜色, 生成 0x000000 到 0xFFFFFF 之间的随机数

		draw_circle(x, y, radius, color, pc, fix.line_length, bytes);  // 绘制矩形

		sleep(1);  // 每秒绘制一次
	}

	/* todo 实际使用时应该添加退出条件（如信号处理） */

	munmap(pc, fix.smem_len);  // 解除内存映射

	close(fb);  // 关闭 framebuffer 设备

	return 0;
}


void draw_point(int x, int y, int color,
				unsigned char *pc,
				size_t line_length,
				size_t bytes)
{
	/* 计算像素点的内存偏移量 */
	unsigned long offset = y * line_length + x * bytes;  // y * 每行字节数 + x * 每像素字节数
	unsigned int *pi = (unsigned int *)(pc + offset);    // 转换为32位指针
	*pi = color;  // 设置像素颜色
}

void draw_circle(int x, int y,
				 int radius, int color,
				 unsigned char *pc,
				 size_t line_length,
				 size_t bytes)
{
	for (int i = x - radius; i <= x + radius; i++)
		for (int j = y - radius; j <= y + radius; j++)
			// 判断像素点是否在圆内  (i - x)² + (j - y)² ≤ r²
			if ((i - x) * (i - x) + (j - y) * (j - y) <= radius * radius)
				draw_point(i, j, color, pc, line_length, bytes);
}
/*
	if ((i - x) * (i - x) + (j - y) * (j - y) <= radius * radius)

	对于一个圆心在 (x, y)，半径为 r 的圆，圆内的点满足：(i - x)² + (j - y)² ≤ r²
*/