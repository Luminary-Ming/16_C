#define _GNU_SOURCE 
#include <stdio.h>
#include <unistd.h>

/**
 * 程序功能：演示 getopt() 函数的基本用法
 * 根据不同的命令行选项输出不同的餐饮信息
 *
 * 支持的选项：
 *   -l  输出 breakfast
 *   -h  输出 lunch
 *   -m  输出 dinner
 *   -i  输出 supper
 *   其他选项或错误格式输出错误信息
 */
int main(int argc, char *argv[])
{
	const char *optstring = "lhim";  // 选项字符组成的字符串 (可以根据需求自定义选项)

	while (1)  // 循环解析命令行选项
	{
		int ret = getopt(argc, argv, optstring);  // 解析命令行选项
		if (ret == -1)  // 检查解析是否完毕
			break;  // 解析完毕, 跳出循环
		switch (ret)
		{
			case 'l': printf("breakfast\n"); break;
			case 'h': printf("lunch\n"); break;
			case 'm': printf("dinner\n"); break;
			case 'i': printf("supper\n"); break;
			case '?': printf("I Don't Know!\n"); break;
			default: break;
		}
	}
	return 0;
}

