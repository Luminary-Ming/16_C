/*
	终端键盘输入的无缓冲、无回显

	w s a d 充当 上 下 左 右
	   空格 充当 下棋
		  q 充当 退出
*/

#include <stdio.h>
#include <termios.h>  // man 3 tcgetattr
#include <unistd.h>   // man 3 tcgetattr

int main(void)
{
	char ch = 0;
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

	while (1)
	{
		ch = getchar();
		switch (ch)
		{
			case 'W':
			case 'w': printf("↑\n"); break;
			case 'S':
			case 's': printf("↓\n"); break;
			case 'A':
			case 'a': printf("←\n"); break;
			case 'D':
			case 'd': printf("→\n"); break;
			case 'Q':
			case 'q': printf("退出!\n"); goto FLAG;
			case ' ': printf("下棋!\n"); break;
		}
	}

FLAG:
	tcsetattr(0, TCSANOW, &old);  // 恢复原始设置

	return 0;
}

/*
	行缓冲模式 :
		输入：w[回车]  →  程序才收到 'w'
		问题：需要按回车，有延迟，不适合实时控制
	无缓冲即时模式：
		输入：w  →  立即响应，不用回车
		效果：适合游戏、实时控制程序
*/