/*
	函数的调用
	实现把十进制数转换成二进制 八进制 十六进制的函数
*/
#include <stdio.h>

void iton(int num, int base);  // 十转所有进制

int main(void)
{
	int num = 255;

	printf("\n%d 的二进制为 : 0b", num);
	iton(num, 2);  // 0b11111111

	printf("\n%d 的八进制为 : 0", num);
	iton(num, 8);  // 0377

	printf("\n%d 的十六进制为 : 0x", num);
	iton(num, 16);  // 0xFF

	return 0;
}

void iton(int num, int base)
{
	if (num == 0)  // 递归终止条件
	{
		printf("0");  // 输入的是 0 返回 0
		return;
	}

	iton(num / base, base);  // 递归先处理高位

	int digit = num % base;

	if (digit < 10)
	{
		printf("%c", '0' + digit);
	}
	else
	{
		printf("%c", 'A' + (digit - 10));
	}
}
/*
	递归

	时间线：
	↓ itox(255) 开始
		↓ 调用 itox(15)
			↓ 调用 itox(0)
			↑ itox(0) 返回（无输出）
		↑ itox(15) 继续执行 → 输出 'F'
		↑ itox(15) 返回
	↑ itox(255) 继续执行 → 输出 'F'
	↑ itox(255) 返回

	最终输出："FF"
*/