/*
	列出 /home/xinming/ 目录下的所有文件和隐藏文件
*/
#include <stdio.h>
#include <glob.h>

int main(int agrc, char *argv[])
{
	glob_t gs;  // 存储解析到的目录个数以及目录名

	if (glob("/home/xinming/*", GLOB_NOSORT, NULL, &gs) != 0)  // 解析目录, 不排序
	{
		fprintf(stderr, "glob() is failed!\n");
		return -1;
	}

	glob("/home/xinming/.*", GLOB_APPEND, NULL, &gs);  // 追加解析隐藏文件

	for (int i = 0; i < gs.gl_pathc; i++)  // 循环遍历目录中的子文件名
		printf("%s\n", gs.gl_pathv[i]);

	globfree(&gs);  // 释放 glob() 开辟的空间

	return 0;
}

