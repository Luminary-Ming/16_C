/*
	字符串函数
	strcpy  复制字符串
*/
#include <stdio.h>
#include <string.h>

char *mystrcpy1(char *dest, const char *src);
char *mystrcpy2(char *dest, const char *src);

int main(void)
{
	const char *src = "hello";
	char dest[10] = "world";

	printf("strcpy = %s\n", mystrcpy2(dest, src));  // strcpy = hello
	printf("dest = %s\n", dest);  // dest = hello

	return 0;
}

char *mystrcpy2(char *dest, const char *src)
{
	char *save = dest;  // 保存 dest 的原始地址

	while (*dest++ = *src++);

	return save;
}

char *mystrcpy1(char *dest, const char *src)
{
	int i = 0;

	for (i = 0; src[i] != '\0'; i++)
	{
		dest[i] = src[i];  // 拷贝有效字符
	}
	dest[i] = '\0'; // 手动添加尾零

	return dest;
}