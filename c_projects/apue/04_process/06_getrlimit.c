#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>

int main(void)
{
	struct rlimit limit;  // 用来存储获取到的进程资源限制的信息

	if (getrlimit(RLIMIT_STACK, &limit) != 0)  // 获取进程资源限制的信息
	{
		perror("getrlimit()");
		return -1;
	}

	printf("Stack Limit : %ldKB\n", limit.rlim_cur >> 10);

	return 0;
}
/*
	Stack Limit : 8192KB
*/