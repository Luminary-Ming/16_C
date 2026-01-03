/*
	输入一个字串，判断它是否是对称串。如 "abcdcba" 是对称串

	思路:就是让正序串和逆序串相等
*/
#include <stdio.h>
#include <string.h>

// 判断是否为对称串
/*
	char* ：指向字符的指针，通过这个指针可以访问整个字符串
	const ：表示这个指针指向的内容是常量，不可修改
	const char *str : 指向常量字符的指针, 通过这个指针可以访问整个字符串（只读）
*/
int is_symmetric(const char *str) {
	// left指针：从字符串开头开始，向右移动
	int left = 0;

	// right指针：从字符串末尾开始，向左移动
	// strlen() 返回的是不包括 '\0' 的字符串长度
	// 长度减1得到最后一个字符的索引位置（ 因为索引从0开始, 0 ~ len-1 ）
	int right = strlen(str) - 1;

	// 当左指针还在右指针左边时，继续比较
	// 如果left >= right，说明已经比较完所有字符对
	while (left < right) {
		// 比较left位置和right位置的字符是否相同
		if (str[left] != str[right]) {
			return 0;  // 发现不相同的字符，立即返回0（不对称）
		}
		left++;  // left指针向右移动一位，指向下一个字符
		right--; // // right指针向左移动一位，指向前一个字符
	}
	return 1;  // 循环结束，说明所有字符对都相同，返回1（对称）
}

int main()
{
	// 定义一个字符数组（字符串），最多可以存储99个字符+1个结束符'\0'
	char str[100];
	printf("请输入一个字符串: ");

	// 读取用户输入的字符串
	// fgets：安全的字符串输入函数
	// 参数1：str -> 存储输入的缓冲区
	// 参数2：sizeof(str) -> 缓冲区大小，防止溢出
	// 参数3：stdin -> 标准输入（键盘输入
	fgets(str, sizeof(str), stdin);

	// 去除字符串末尾的换行符
	// fgets会读取换行符，我们需要将其替换为字符串结束符'\0'
	// strcspn(str, "\n")：查找换行符在字符串中的位置
	// 如果找到换行符，将其替换为'\0'
	str[strcspn(str, "\n")] = '\0';

	if (is_symmetric(str)) {
		printf("\"%s\" 是对称串\n", str);
	}
	else {
		printf("\"%s\" 不是对称串\n", str);
	}

	return 0;
}