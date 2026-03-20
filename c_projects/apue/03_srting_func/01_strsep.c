#include <stdio.h>
#include <string.h>

int main(void)
{
	char buf[] = "Hello       World!";  // 要分割的字符串
	char *delim = " ";  // 分隔符
	char *str = buf;  // 代替 buf 作操作
	
	while (1)
	{
		char *p = strsep(&str, delim);  // 获取分割字符串的子串
		if (p == NULL)
			break;
		printf("%s\n", p);
	}
}
/*
打印：
	Hello




	World!

	strsep 遇到连续分隔符将返回空字符串 ""
	原理是修改原字符串把分隔符替换成 '\0'
	空字段会保留 如 a,,b 会返回 "a", "", "b"
*/