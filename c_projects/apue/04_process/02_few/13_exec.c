#include <stdio.h>
#include <unistd.h>

int main(void)
{
	printf("Begin...\n");  // 打印开始

	execl("/usr/bin/ls", "ls", "-l", NULL);  // 使用 ls -l 替换当前进程

	printf("End...\n");  // 打印结束

	return 0;
}
/*
	xinming@04_process$ ./a.out
	Begin...
	总计 104
	-rw-rw-r-- 1 xinming xinming   681  3月 24 10:34 01_exit.c
	-rw-rw-r-- 1 xinming xinming   389  3月 24 14:49 08_getpid_getppid.c
	-rw-rw-r-- 1 xinming xinming  2453  3月 24 14:49 09_fork.c
	-rw-rw-r-- 1 xinming xinming  1605  3月 24 15:20 10_wait.c
	-rw-rw-r-- 1 xinming xinming  1100  3月 24 15:47 11_concurrent_calculate_prime.c
	-rw-rw-r-- 1 xinming xinming   791  3月 24 16:58 12_copy_on_write.c
	-rw-rw-r-- 1 xinming xinming   239  3月 24 16:59 13_exec.c
	-rwxrwxr-x 1 xinming xinming 16008  3月 24 16:59 a.out
	drwxrwxr-x 3 xinming xinming  4096  3月 23 11:42 bin
	-rw-rw-r-- 1 xinming xinming   267  3月 23 11:52 main.c
	drwxrwxr-x 3 xinming xinming  4096  3月 23 11:42 obj
	xinming@04_process$
*/