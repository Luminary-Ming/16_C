/*
	int atexit(void (*function)(void));
	成功返回 0，失败返回非 0 (不设置 erron)
	atexit() 用于注册一个函数，该函数会在程序正常终止时被自动调用
*/
#include <stdlib.h>
#include <stdio.h>

void close_file(void);
void free_mem(void);
void exit_begin(void);

int main(void)
{
	if (atexit(close_file) != 0)  // 注册终止函数
	{
		fprintf(stderr, "register close_file() is failed\n");
		exit(1);  // 由于注册失败 close_file() 失败, 终止进程返回状态 1
	}

	if (atexit(free_mem) != 0)  // 注册终止函数
	{
		fprintf(stderr, "register free_mem() is failed\n");
		exit(2);  // 由于注册失败 free_mem() 失败, 终止进程返回状态 2
	}

	if (atexit(exit_begin) != 0)  // 注册终止函数
	{
		fprintf(stderr, "register exit_begin() is failed\n");
		exit(3);  // 由于注册失败 exit_begin() 失败, 终止进程返回状态 3
	}

	printf("程序主逻辑执行");

	return 0;  // 程序正常终止
}

void close_file(void)
{
	printf("善后[1] 关闭打开的文件!\n");
}

void free_mem(void)
{
	printf("善后[2] 释放开辟的空间!\n");
}

void exit_begin(void)
{
	printf("善后[3] 开始进行终止处理程序!\n");
}

/*
	输出：
	程序主逻辑执行
	善后[3] 开始进行终止处理程序!  // 注意 : 后注册的先调用 (LIFO)
	善后[2] 释放开辟的空间!
	善后[1] 关闭打开的文件!
*/
