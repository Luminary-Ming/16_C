/*
	从键盘输入长度不等的两个字串，将长串连接于短串之后输出。
*/
#include <stdio.h>
#include <string.h>

int main(void)
{
	char str1[100], str2[100];

	printf("请输入第一个字符串: ");
	fgets(str1, sizeof(str1), stdin);
	str1[strcspn(str1, "\n")] = '\0';

	printf("请输入第二个字符串: ");
	fgets(str2, sizeof(str2), stdin);
	str2[strcspn(str2, "\n")] = '\0';

	int len1 = strlen(str1);
	int len2 = strlen(str2);

	if (len1 > len2) {
		printf("字符串 %s 较短, 长度是 %d\n", str1, len1);
		strcat(str2, str1);  // 将 str1 连接到 str2 后面
		printf("将短串连接于长串之后 : %s", str2);
	}
	else if (len1 < len2) {
		printf("字符串 %s 较短, 长度是 %d", str2, len2);
		strcat(str1, str2);  // 将 str2 连接到 str1 后面
		printf("将短串连接于长串之后 : %s", str1);
	}
	else {
		printf("字符串 %s 和字符串 %s 一样长, 长度是 %d", str1, str2, len1);
	}

	return 0;

	return 0;
}