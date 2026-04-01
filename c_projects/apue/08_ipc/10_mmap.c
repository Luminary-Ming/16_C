#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define LENGTH 4096

int main(void)
{
	int ret = 0;  // 返回状态码

	// 映射共享内存
	void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED)
	{
		perror("mmap()");
		ret = -1;
		goto ERR_1;
	}

	// 创建子进程
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork()");
		ret = -2;
		goto ERR_2;
	}

	if (pid == 0)  // 子进程
	{
		memcpy(ptr, "Hello", 5);  // 内存拷贝数据
		munmap(ptr, LENGTH);  // 解除映射
		exit(0);  // 正常终止
	}
	else  // 父进程
	{
		wait(NULL); // 等待子进程结束
		puts(ptr);  // 把共享内存中的数据打印到 stdout 上
	}

ERR_2:
	munmap(ptr, LENGTH);  // 解除映射

ERR_1:
	return ret;
}