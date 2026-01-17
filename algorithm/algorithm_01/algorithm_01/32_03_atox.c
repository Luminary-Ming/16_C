/*
	实现 myatox() 功能, 把字符串转为十六进制
	"0x48000000"  ->  0x48000000
	"0X6789ABCD"  ->  0X6789ABCD
*/
#include <stdio.h>
#include <string.h>

void myatox(char *str);

int main(void)
{
	char str[100];
	printf("\n请输入字符串: ");
	fgets(str, sizeof(str), stdin);
	str[strcspn(str, "\n")] = '\0';

	while (5)
	{

	}


	return 0;
}

void myatox(char *str) {
}