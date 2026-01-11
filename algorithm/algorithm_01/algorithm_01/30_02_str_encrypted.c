/*
	输入一行小写英文字母，按如下规则加密：
	a → c、 b → d、x → z、y → a、z → b
	试写加密程序。
*/
#include <stdio.h>
#include <string.h>

char *encipher(char *str);

int main(void)
{
	char str[100];
	printf("请输入一行小写英文字母: ");
	fgets(str, sizeof(str), stdin);
	str[strcspn(str, "\n")] = '\0';

	printf("加密后: %s", encipher(str));
	return 0;
}

char *encipher(char *str) 
{
	int len = strlen(str);

	for (int i = 0; i < len; i++)
	{
		if (str[i] >= 'a' || str[i] <= 'z')  // 判断是否是小写字母 
		{
			str[i] = 'a' + (str[i] + 2 - 'a') % 26;
		}
	}
	return str;
}
/*
	% 26 实现了自动循环：

	当数字 < 26 时：保持不变（5 % 26 = 5）

	当数字 ≥ 26 时：回到0开始（26 % 26 = 0, 27 % 26 = 1）
*/