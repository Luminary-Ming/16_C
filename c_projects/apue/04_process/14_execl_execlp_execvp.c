#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	char *str[] = { "ls", "-l", NULL };  // 把选项参数列表合到了一起

	printf("Begin...\n");

	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork()");
		exit(1);
	}

	if (pid == 0)  // 子进程
	{
		//execl("/usr/bin/ls", "ls", "-l", NULL);  // 使用 ls -l 替换当前进程
		//execlp("ls", "ls", "-l", NULL);
		execvp("ls", str);
		perror("execvp()");
		exit(2);
	}

	wait(NULL);  // 等待子进程结束
	printf("End...\n");
	return 0;
}
/*
	xinming@04_process$ ./a.out
	Begin...
	总计 108
	-rw-rw-r-- 1 xinming xinming   681  3月 24 10:34 01_exit.c
	-rw-rw-r-- 1 xinming xinming  1452  3月 24 10:34 02_01_atexit.c
	-rw-rw-r-- 1 xinming xinming  1126  3月 24 10:34 02_02_atexit.c
	-rw-rw-r-- 1 xinming xinming   791  3月 24 16:58 12_copy_on_write.c
	-rw-rw-r-- 1 xinming xinming  1027  3月 24 17:10 13_exec.c
	-rw-rw-r-- 1 xinming xinming   463  3月 24 17:10 14_demo.c
	-rwxrwxr-x 1 xinming xinming 16168  3月 24 17:11 a.out
	drwxrwxr-x 3 xinming xinming  4096  3月 23 11:42 bin
	-rw-rw-r-- 1 xinming xinming   267  3月 23 11:52 main.c
	drwxrwxr-x 3 xinming xinming  4096  3月 23 11:42 obj
	End...
	xinming@04_process$
*/