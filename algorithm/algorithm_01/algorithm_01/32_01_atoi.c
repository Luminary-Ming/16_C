/*
	实现 myatoi() 函数, 字符串形式的数字转为整型数字
*/
#include <stdio.h>
#include <string.h>

int myatoi1(const char *str);
int myatoi2(const char *str);

int main(void)
{
	char str[100];

	while(10)
	{
		printf("\n请输入字符串: ");
		fgets(str, sizeof(str), stdin);
		str[strcspn(str, "\n")] = '\0';

		printf("\"%s\" -> %d", str, myatoi2(str));
	}
	return 0;
}

int myatoi2(const char *str)
{
	int i = 0;
	int num = 0;
	int flag = 1;  // 处理正负

	while (str[i] == ' ') i++;  // 跳过空格

	if (str[i] == '+' || str[i] == '-')
	{
		flag = str[i] == '-' ? -1 : 1;
		i++;
	}

	while (str[i] != '\0')
	{
		switch (str[i])
		{
			case '0'...'9': 
				num *= 10;
				num += str[i] - '0';
				break;
			default:
				return num * flag;
		}
		i++;
	}
	return num * flag;
}
/*
	myatoi2 成功示例:
	"123" -> 123
	"abc" -> 0
	"-123abc" -> -123
	"---abc123" -> 0
	"---123" -> 0
	"000-123" -> 0
	"+++0123" -> 0
	"   -123" -> -123
	"123   456" -> 123
	"-123+456" -> -123
*/

int myatoi1(const char *str)
{
	int num = 0;  // 返回的数字

	// 1. 跳过开头空白字符
	size_t skip_blank = strspn(str, " \t\n\r");
	str += skip_blank;

	// 2. 检查是否有效内容 (从开头连续都不包括 "+-0123456789" 直接返回 0)
	size_t effective = strspn(str, "+-0123456789");
	if (effective == 0) return 0;

	// 跳过开头所有的 0
	size_t skip_zero = strspn(str, "0");
	str += skip_zero;

	if(strspn(str, "+-"))  // 开头带正负号的
	{
		if (strspn(str, "-") % 2 != 0)  // 开头奇数个负号
		{
			str += strspn(str, "-");  // 跳过前面的负号

			if (strspn(str, "0123456789") == 0) return 0;

			for (int i = 0; i < strspn(str, "0123456789"); i++)  // 只包括数字的部分(防止这种的 "---12+345" "-123abc" )
			{
				num = num * 10 + (str[i] - '0');
			}
			return num ? -num : 0;  // 结果是负数 (0 就是 0 了)
		}
		else  // 开头是偶数个负号 和 开头是任意个正号 (总之都是正数)
		{
			str += strspn(str, "+-");  // 跳过前面的正负号

			if (strspn(str, "0123456789") == 0) return 0;

			for (int i = 0; i < strspn(str, "0123456789"); i++)
			{
				num = num * 10 + (str[i] - '0');
			}
			return num;
		}
	}
	else  // 开头不带正负号的
	{
		for (int i = 0; i < strspn(str, "0123456789"); i++) 
		{
			num = num * 10 + (str[i] - '0');
		}
		return num;
	}
	return 0;
}
/*
	myatoi1 成功示例:
	"123" -> 123
	"abc" -> 0
	"123abc" -> 123
	"abc123" -> 0
	"-123abc" -> -123
	"---abc123" -> 0
	"---123" -> -123
	"----123" -> 123
	"000-123" -> -123
	"+++0123" -> 123
	"   -123" -> -123
	"-123+456" -> -123
	"-----123--++456" -> -123
	"123   456" -> 123
*/