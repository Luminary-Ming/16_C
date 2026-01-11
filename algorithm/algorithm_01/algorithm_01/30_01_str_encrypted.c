/*
	输入一行字符串，按如下规则加密：
	如果是英文字母则大写变小写、小写变大写，
	对非英文字符则保持不变。试写加密程序。
*/

#include <stdio.h>
#include <string.h>

char *encipher(char *str);  // 加密字符串

int main(void)
{
	char str[100];

	printf("请输入一个字符串: ");

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
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] = str[i] - 'a' + 'A';
		}
		else if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] = str[i] - 'A' + 'a';
		}else{}
	}
	return str;
}