#include <stdio.h>

// main函数, 程序入口点
// argc: 命令行参数个数 (包括程序名)
// argv: 命令行参数字符串数组
// env: 环境变量字符串数组
int main(int argc, char *argv[], char *env[])
{
	printf("argc = %d\n", argc);  // 打印命令行参数的数量

	// 循环遍历所有命令行参数
	for (int i = 0; i < argc; i++)
		puts(argv[i]);  // 打印第i个命令行参数，自动添加换行符
	printf("\n");

	// 循环遍历所有环境变量, env 数组以 NULL 指针作为结束标志
	for (int i = 0; env[i] != NULL; i++)
		puts(env[i]);
	printf("\n");

	return 0;
}
