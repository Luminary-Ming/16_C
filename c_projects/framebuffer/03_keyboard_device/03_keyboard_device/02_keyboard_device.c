/*
	查找驱动键盘设备, 从 Linux 内核读取键盘原始输入
	
	1.确定键盘驱动设备是哪个外设
		[1] 从 /dev/input 目录中用 cat 命令一个个尝试哪一个是键盘设备
		[2] 从 /proc/bus/input 目录中查找 devices 确定当前机器使用的是什么 event 设备
	2.使用 open 打开键盘设备文件 (只读方式打开, 只是检测键盘有没有被按下, 并不是修改键盘)
	3.通过 read() 读取键盘设备
	4.找到相应的键值做相应的操作
*/

#include <sys/types.h>  // man 2 open
#include <sys/stat.h>   // man 2 open
#include <fcntl.h>      // man 2 open
#include <stdio.h>
#include <unistd.h>     // man 2 read
#include <linux/input.h>   // struct input_event 结构体

int main(void)
{
	int kb_fd = open("/dev/input/event1", O_RDONLY);  // 以只读方式打开键盘设备文件
	if (kb_fd < 0)
	{
		perror("open()");
		return -1;
	}

	struct input_event ev;  // ev 存储读取键盘的数据
	while (1)
	{
		read(kb_fd, &ev, sizeof(ev));  // 读取键盘设备数据
		if (ev.type == EV_KEY)  // 判断是否按键
			if (ev.value == SYN_REPORT)
			{
				switch (ev.code)
				{
					case 103: printf("前进 !"); break;
					case 108: printf("后退 !"); break;
					case 105: printf("左转 !"); break;
					case 106: printf("右转 !"); break;
					case  28: printf("开炮 !"); break;
					case   1: printf("结束 !"); break;
				}
			}
	}


}