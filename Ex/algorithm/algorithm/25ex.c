/*
	从键盘输入两个字串，输出其中较短的那个字串，并输出它的长度
*/

#include <stdio.h>
#include <string.h>

int main(void)
{
	char str1[100], str2[100];

	printf("请输入一个字符串: ");
	fgets(str1, sizeof(str1), stdin);
	str1[strcspn(str1, "\n")] = '\0';

	printf("请输入另一个字符串: ");
	fgets(str2, sizeof(str2), stdin);
	str2[strcspn(str2, "\n")] = '\0';

	int len1 = strlen(str1);
	int len2 = strlen(str2);

	if (len1 < len2) {
		printf("字符串 %s 较短, 长度是 %d", str1, len1);
	}
	else if(len1 == len2){
		printf("字符串 %s 和字符串 %s 一样长, 长度是 %d", str1, str2, len1);
	}
	else {
		printf("字符串 %s 较短, 长度是 %d", str2, len2);
	}

	return 0;
}