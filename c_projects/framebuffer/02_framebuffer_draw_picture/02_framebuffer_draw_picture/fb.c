/*
	用 framebuffer 设备在屏幕上显示一张图片
*/
#include <stdio.h>
#include <sys/types.h>  // man 2 open
#include <sys/stat.h>   // man 2 open
#include <fcntl.h>      // man 2 open
#include <unistd.h>     // man 2 close
#include <sys/ioctl.h>  // man 2 ioctl
#include <linux/fb.h>   // 有关 framebuffer 的头文件
#include <sys/mman.h>   // man 2 mmap
#include "pic.h"

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
	功能 : 绘制图片
	   x, y  像素点的坐标
	 logo_w  图片宽度
	 logo_h  图片高度
	      p  指向图片像素数据的数组
		 pc  帧缓冲内存指针
line_length  每行像素所占的字节数
	  bytes  单个像素点所占的字节数
*/
void draw_pic(int x, int y, int logo_w, int logo_h, 
			  const unsigned char *p, 
			  unsigned char *pc, 
			  size_t line_length, size_t bytes);

int main(void)
{
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

	int x = 100;  // 左上角点 x 坐标
	int y = 100;  // 左上角点 y 坐标
	int logo_w = 1024;  // 图片宽
	int logo_h = 600;   // 图片高

	// gImage_pic 数组存储的是一张 1024×600 像素的 RGB888 格式图片的像素数据
	draw_pic(x, y, logo_w, logo_h, gImage_pic, pc, fix.line_length, bytes);

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


void draw_pic(int x, int y, int logo_w, int logo_h,
			  const unsigned char *p,
			  unsigned char *pc,
			  size_t line_length, size_t bytes)
{
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;
	unsigned int color = 0;
	
	for (int j = y; j < (y + logo_h); j++)
	{
		for (int i = x; i < (x + logo_w); i++)
		{
			red = *p++;
			green = *p++;
			blue = *p++;
			color = (red << 16) | (green << 8) | blue;
			draw_point(i, j, color, pc, line_length, bytes);
		}
	}
}

/*
	gImage_pic 数组存储的是一张 1024×600 像素的 RGB888 格式图片的像素数据
	const unsigned char gImage_pic[1843200] = {
	0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10,
	0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD,
	0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9, 0X10, 0XBD, 0XF9,
	.......

	0X10, 0XBD, 0XF9,  // 第一个像素：R=0x10, G=0xBD, B=0xF9
	0X10, 0XBD, 0XF9,  // 第二个像素：R=0x10, G=0xBD, B=0xF9
	0X10, 0XBD, 0XF9,  // 第三个像素：R=0x10, G=0xBD, B=0xF9
	.......

	red = *p++;     // 0X10  0001 0000
	green = *p++;   // 0XBD  1011 1101
	blue = *p++;    // 0XF9  1111 1001
	color = (red << 16) | (green << 8) | blue;

	 red << 16  00010000 00000000 00000000
	green << 8  00000000 10111101 00000000
		  blue  00000000 00000000 11111001

	(red << 16) | (green << 8) | blue  ->  00010000 10111101 11111001
*/