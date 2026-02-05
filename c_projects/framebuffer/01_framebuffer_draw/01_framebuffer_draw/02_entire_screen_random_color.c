/*
	整个屏幕填充随机颜色, 每秒切换一次
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

int main(void)
{
	srand(getpid());  // 设置随机数的种子

	int fb = open("/dev/fb0", O_RDWR);   // 以读写的形式打开 framebuffer 设备
	if (fb < 0)  // 判断打开 framebuffer 设备是否失败
	{
		perror("open()");
		return -1;
	}

	struct fb_var_screeninfo var;  // 可变屏幕信息的结构体变量
	// FBIOGET_VSCREENINFO 表示获取可变屏幕信息
	int ret = ioctl(fb, FBIOGET_VSCREENINFO, &var);  // 获取帧缓冲设备的可变屏幕信息, 复制到 var 中
	if (ret < 0)
	{
		perror("ioctl()");
		close(fb);
		return -2;
	}

	size_t len = var.xres * var.yres * var.bits_per_pixel >> 3;  // 帧缓冲的总字节大小
	// MAP_SHARED 创建共享的内存映射
	unsigned char *pc = (unsigned char *)mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);  // 把 framebuffer 映射到内存中
	if (pc == (void *)(-1))  // 判断把 framebuffer 映射到内存中是否失败
	{
		perror("mmap()");
		close(fb);
		return -3;
	}

	unsigned int *pi = (unsigned int *)pc;

	while (1)
	{
		int color = rand() % 0xFFFFFF;  // 随机颜色, 生成 0x000000 到 0xFFFFFF 之间的随机数
		for (int y = 0; y < var.yres; y++)
			for (int x = 0; x < var.xres; x++)
				pi[y * var.xres + x] = color;  // 遍历所有像素点, 给每个像素点填充颜色
		sleep(1);  // 每秒绘制一次
	}

	munmap(pc, len);  // 解除内存映射

	close(fb);  // 关闭 framebuffer 设备

	return 0;
}

/*
	for (int y = 0; y < var.yres; y++)
			for (int x = 0; x < var.xres; x++)
				pi[y * var.xres + x] = color;

	因为每行有"宽度"个像素，要到达第 y 行，需要跳过 y × "宽度"个像素


	屏幕：10 × 6

	内存布局（正确顺序）：
	行0: [00] [01] [02] [03] [04] [05] [06] [07] [08] [09]
	行1: [10] [11] [12] [13] [14] [15] [16] [17] [18] [19]
	行2: [20] [21] [22] [23] [24] [25] [26] [27] [28] [29]
	行3: [30] [31] [32] [33] [34] [35] [36] [37] [38] [39]
	行4: [40] [41] [42] [43] [44] [45] [46] [47] [48] [49]
	行5: [50] [51] [52] [53] [54] [55] [56] [57] [58] [59]


	pi[i * 1080 + j] 和 pi[j * 1920 + i] 的巨大区别：

	对比	  pi[i * 1080 + j]  	pi[j * 1920 + i]
	公式	  x × 高度 + y	        y × 宽度 + x
	逻辑	  先按列处理，再按行	先按行处理，再按列
	结果	  像素位置完全错乱	    像素位置正确对应
	屏幕效果  条纹/网格等奇怪图案	均匀的纯色填充
*/