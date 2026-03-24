#include <stdio.h>
#include <unistd.h>

// 声明外部全局变量 environ
// environ 是系统定义的环境变量指针数组，在 unistd.h 中声明
// 以 NULL 指针作为数组结束标志，每个元素指向 "KEY=value" 格式的字符串
extern char **environ;

// main 函数，程序入口点
// argc: 命令行参数个数（包括程序名）
// argv: 命令行参数字符串数组，以 NULL 结尾
// 注意：这里没有第三个参数 env，而是使用全局变量 environ
int main(int argc, char *argv[])
{
	printf("argc = %d\n", argc);

	for (int i = 0; i < argc; i++)
		puts(argv[i]);
	printf("\n");

	// 循环遍历所有环境变量
	// environ 是全局指针数组，最后一个元素为 NULL
	for (int i = 0; environ[i] != NULL; i++)
		puts(environ[i]);
	printf("\n");

	return 0;
}
