/*
	输入一行英文，已知各单词之间用1个空格相隔（第一个单词前没有空格)
	统计这行英文有多少个单词。
*/
#include <stdio.h>
#include <string.h>

int count_words(const char *str);  // 统计单词

int main(void)
{
	char str[100];
	printf("请输入一行英文: ");
	fgets(str, sizeof(str), stdin);
	str[strcspn(str, "\n")] = '\0';
	printf("\n");
	printf("\n这行英文共有 %d 个单词", count_words(str));
	return 0;
}

int count_words(const char *str) 
{
	int count = 0;
	char *token = strtok(str, " ");  // 分割字符串

	while (token)
	{
		count++;
		printf("单词: %s\n", token);
		token = strtok(NULL, " ");
	}
	return count;
}