/*
	输入一个字串，判断它是否是对称串。如 "abcdcba" 是对称串

	思路:就是让正序串和逆序串相等
*/
#include <stdio.h>
#include <string.h>

int is_symmetry(const char *str);  // 判断是否对称

int main(void)
{
	char str[100];

	printf("请输入一个字符串: ");

	fgets(str, sizeof(str), stdin);  // 读取用户输入的字符串, 包括空白符

	str[strcspn(str, "\n")] = '\0';  // 去掉换行符

	if (is_symmetry(str))
	{
		printf("\"%s\" 是对称串\n", str);
	}
	else
	{
		printf("\"%s\" 不是对称串\n", str);
	}

	return 0;
}

int is_symmetry(const char *str)
{
	int len = strlen(str);  // 获取字符串长度

	int left = 0;  // 左指针
	int right = len - 1;  // 右指针

	while (left < right)
	{
		if (str[left] != str[right]) {
			return 0;  // 不是对称串
		}
		left++;
		right--;
	}
	return 1;  // 是对称串
}
	
	