/*
	 画一个圆心 (200, 200) 半径为 100 的实心圆
	 让圆移动起来, 碰到边界回弹变颜色
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
#include <sys/time.h>
#include <signal.h>

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

// 全局标志，用于控制程序运行  volatile 防止编译器优化
volatile sig_atomic_t keep_running = 1;

void signal_handler(int sig)
{
	keep_running = 0;
}

// 定时器信号处理函数
void timer_handler(int none)
{
	// 什么也不用做，只是为了接收信号
	// 当定时器到期时，内核会发送 SIGALRM 信号给进程
}

int main(void)
{
	// 设置信号处理
	signal(SIGINT, signal_handler);  // Ctrl+C
	signal(SIGTERM, signal_handler);  // kill命令
	signal(SIGALRM, timer_handler);   // 新增：定时器信号

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

	// 定时器结构体
	struct itimerval timer;  

	// 初始延迟：10毫秒后第一次触发
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 10000;  // 10000微秒 = 10毫秒

	// 间隔时间：每10毫秒触发一次
	// it_interval 表示：第一次触发后，每隔多久再触发一次
	// 如果设为0，表示只触发一次（单次定时器）
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 10000;

	// 设置定时器
	// ITIMER_REAL: 使用真实时间
	// &timer: 定时器配置
	// NULL: 不要保存旧的定时器设置
	if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
	{
		perror("setitimer()");
		munmap(pc, fix.smem_len);
		close(fb);
		return -5;
	}

	int dir = rand() % 4 + 1;  // 随机 1 ~ 4 方向
	int flag = 0;  
	int color = rand() % 0xFFFFFF;  // 随机颜色, 生成 0x000000 到 0xFFFFFF 之间的随机数

	while (keep_running)  // 持续绘制随机颜色的矩形
	{
		

		flag = dir;

		draw_circle(x, y, radius, 0x0, pc, fix.line_length, bytes);

		switch (dir)
		{
			case 1: if ((x + radius) == (var.xres - 1) && (y == radius)) dir = 3;
				  else if ((x + radius) == (var.xres - 1)) dir = 2;
				  else if ((y == radius)) dir = 4;
				break;
			case 2: if ((x == radius) && (y == radius)) dir = 4;
				  else if(x == radius) dir = 1;
				  else if (y == radius) dir = 3;
				break;
			case 3: if ((x == radius) && (y + radius == var.yres - 1)) dir = 1;
				  else if ((x == radius)) dir = 4;
				  else if ((y + radius) == var.yres - 1) dir = 2;
				break;
			case 4: if ((x + radius) == var.xres - 1 && (y + radius) == var.yres - 1) dir = 2;
				  else if ((x + radius) == var.xres - 1) dir = 3;
				  else if ((y + radius) == var.yres - 1) dir = 1;
				break;
		}
		if (flag != dir)
			color = rand() % 0xFFFFFF;

		switch (dir)
		{
			case 1: x++, y--; break;
			case 2: x--, y--; break;
			case 3: x--, y++; break;
			case 4: x++, y++; break;
		}

		draw_circle(x, y, radius, color, pc, fix.line_length, bytes);  // 绘制矩形

		pause();   // 阻塞, 等待信号(定时器信号 Ctrl + c信号 kill信号....)
	}

	// 程序结束时清理定时器
	struct itimerval stop_timer = { {0, 0}, {0, 0} };
	setitimer(ITIMER_REAL, &stop_timer, NULL);

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