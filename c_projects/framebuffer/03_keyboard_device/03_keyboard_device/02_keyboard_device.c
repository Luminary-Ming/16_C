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
#include <termios.h>

int main(void)
{
	int kb_fd = open("/dev/input/event1", O_RDONLY);  // 以只读方式打开键盘设备文件
	if (kb_fd < 0)
	{
		perror("open()");
		return -1;
	}

	// 终端配置结构体
	struct termios old, new;  // old 存储原有的配置, new 存储修改后的配置
	tcgetattr(0, &old);  // 获取当前终端设置（0=stdin）
	new = old;  // 把获取的原有配置复制给 new
	// c_lflag 本地模式标志
	new.c_lflag &= ~ICANON;  // 关闭规范模式
	new.c_lflag &= ~ECHO;  // 关闭回显
	tcsetattr(0, TCSANOW, &new);  // 应用新设置( 关闭规范模式, 关闭回显 )
	//   TCSANOW: 立即生效
	// TCSADRAIN: 等所有输出完成
	// TCSAFLUSH: 清空输入输出缓冲区后生效

	struct input_event ev;  // ev 存储读取键盘的数据
	while (1)
	{
		read(kb_fd, &ev, sizeof(ev));  // 读取键盘设备数据
		if (ev.type == EV_KEY)  // 判断是否按键
			// ev.value: 0=释放, 1=按下, 2=长按
			if (ev.value == SYN_CONFIG)
			{
				switch (ev.code)
				{
					case 103: printf("前进 !\n"); break;  // ↑
					case 108: printf("后退 !\n"); break;  // ↓
					case 105: printf("左转 !\n"); break;  // ←
					case 106: printf("右转 !\n"); break;  // →
					case  28: printf("开炮 !\n"); break;  // 回车
					case   1: printf("结束 !\n"); goto FLAG;  // ESC
				}
			}
	}
FLAG:
	tcsetattr(0, TCSANOW, &old);  // 恢复原始设置
	close(kb_fd);  // 关闭文件描述符

	return 0;
}