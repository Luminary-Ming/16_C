#include <stdio.h>
#include <string.h>

#define CONTEXT_MAX 1000
#define FILENAME "hello.txt"

// 保存数据到文件
void saveFile(const char *content);
// 文件中读取数据
void readFile(char *content, int content_size);

int main(void)
{
	char content[CONTEXT_MAX] = "";
	char input[256];  // 接收输入的字符数组
	
	readFile(content, CONTEXT_MAX);  // 读文件

	if (strlen(content) > 0)
	{
		printf("\n=== 之前保存的内容 ===\n");
		printf("%s\n", content);
		printf("\n======================\n");
	}

	printf("输入新的要保存的内容(输入 'q' 退出, 输入 'wq' 保存退出) :\n");

	while (1)
	{
		printf("> ");
		fgets(input, sizeof(input), stdin);  // 输入
		input[strcspn(input, "\n")] = '\0';  // 清除换行符

		if (strcmp(input, "q") == 0)
		{
			printf("已退出程序。\n");
			break;
		}
		else if (strcmp(input, "wq") == 0)
		{
			saveFile(content);
			break;
		}
		else
		{
			if (strlen(content) + strlen(input) + 2 < CONTEXT_MAX)  // 已存的 + 新输入的 + 换行 + 尾零 < 总大小
			{
				strcat(content, input);
				strcat(content, "\n");  // 换行
				printf("已添加到缓存。\n");
			}
			else
				printf("错误：内容过长！\n");
		}
	}
	return 0;
}


// 保存数据到文件
void saveFile(const char *content)
{
	FILE *file = fopen(FILENAME, "w");  // w 写
	if (file == NULL)
	{
		printf("错误 : 无法创建 !");
	}

	fprintf(file, "%s", content);
	fclose(file);
	printf("内容已保存到 %s\n", FILENAME);
}
// 文件中读取数据
void readFile(char *content, int content_size)
{
	FILE *file = fopen(FILENAME, "r");  // r 读
	if (file == NULL)
	{
		printf("第一次读取，文件不存在。\n");
		return;
	}

	size_t bytesRead = fread(content, 1, content_size - 1, file);
	content[bytesRead] = '\0';  // 确保字符串结束

	fclose(file);
	printf("已加载上次保存的内容。\n");
}