/*
	从键盘输入两个字符串，判断它们是否相同
*/
#include <stdio.h>
#include <string.h>

// 判断是否为相同字符串
/*
	char* ：指向字符的指针，通过这个指针可以访问整个字符串
	const ：表示这个指针指向的内容是常量，不可修改
	const char *str : 指向常量字符的指针, 通过这个指针可以访问整个字符串（只读）
*/
int is_same(const char *str1, const char *str2) {
	// 字符串长度  strlen() 返回的是不包括 '\0' 的字符串长度
	// 长度不同，肯定不是相同字符串
	if (strlen(str1) != strlen(str2)) {
		return 0;
	}

	int i = 0;
	while (str1[i] != '\0') {
		if (str1[i] != str2[i]) {
			return 0;  // 发现不相同的字符，立即返回0（不相同）
		}
		i++;
	}

	return 1;  // 循环结束, 是相同的字符
}

int main(void) 
{
	char str1[100], str2[100];  // 定义两个字符数组

	printf("请输入一个字符串: ");

	// 读取用户输入的字符串
	// fgets：安全的字符串输入函数
	// 参数1：str -> 存储输入的缓冲区
	// 参数2：sizeof(str) -> 缓冲区大小，防止溢出
	// 参数3：stdin -> 标准输入 键盘输入
	fgets(str1, sizeof(str1), stdin);

	// 去除字符串末尾的换行符
	// fgets会读取换行符，我们需要将其替换为字符串结束符'\0'
	// strcspn(str, "\n")：查找换行符在字符串中的位置
	// 如果找到换行符，将其替换为'\0'
	str1[strcspn(str1, "\n")] = '\0';

	printf("请输入另一个字符串: ");

	fgets(str2, sizeof(str2), stdin);
	str2[strcspn(str2, "\n")] = '\0';


	if (is_same(str1, str2)) {
		printf("\"%s\" 和 \"%s\" 是相同字符串\n", str1, str2);
	}
	else {
		printf("\"%s\" 和 \"%s\" 是不同字符串\n", str1, str2);
	}

	return 0;
}