/*
	设计一个程序, 输入一个字符串以 # 结尾
	输出此字符串中连续出现最长的数字串及其开始的下标
	例如: ab125ef1234567#
	输出：1234567 开始的下标为：8
*/
#include <stdio.h>

void func(char *str)
{
	int cur_len = 0;  // 当前连续数字串的长度
	int cur_index = -1;  // 当前连续数字串开始的下标
	int max_len = 0;  // 最长的连续数字串的长度
	int max_index = -1;  // 最长的连续数字串开始的下标

	int i = 0;  // 字符串下标
	while (str[i] != '#')  // 遍历字符串, 直到遇到 '#'
	{
		if (str[i] >= '0' && str[i] <= '9')  // 如果当前字符是数字
		{
			if (cur_len == 0)  // 如果是新开始的连续数字串
				cur_index = i;  // 更新下标
			cur_len++;  // 长度++

			if (cur_len > max_len)  // 更新最长记录
			{
				max_len = cur_len;
				max_index = cur_index;
			}
		}
		else
		{
			cur_len = 0;  // 当前字符不是数字, 重置长度
			cur_index = -1;  // 重置下标
		}
		i++;  // 遍历下一个字符	
	}

	if (max_len > 0)
	{
		for (i = 0; i < max_len; i++)
			printf("%c", str[max_index + i]);
		printf("\n开始的下标为：%d\n", max_index);
	}
	else
		printf("没有符合的数字串\n");
}

