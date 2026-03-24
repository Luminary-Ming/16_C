#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
	int num = 10;  // 定义一个整型变量, 为了验证父子进程的独立性

	printf("调用 fork(2) 之前只有父进程 (PID:%d), num = %d\n", getpid(), num);

	pid_t pid = fork();  // 创建子进程
	if (pid < 0)  // 检查创建子进程是否失败
	{
		perror("fork()");
		exit(1);
	}

	//======= fork(2) 成功后, 父子进程从这里开始分叉执行了 =======
	
	if (pid == 0)  // 子进程的操作
	{
		num += 5;  // 子进程修改自己的 num 变量
		printf("I am child process (PID:%d, PPID:%d) num = %d\n", getpid(), getppid(), num);
	}
	else  // 父进程的操作
	{
		num -= 5;  // 父进程修改自己的 num 变量
		printf("I am parent process (PID:%d, child process PID:%d) num = %d\n", getpid(), pid, num);
		wait(NULL);  // 父进程阻塞, 等待子进程结束 (收尸)
	}

	// 父子进程都会执行这行代码 (同一份代码, 父子进程各自执行一份)
	printf("PID:%d Game Over num = %d\n", getpid(), num);

	exit(0);
}
/*
	打印 :
	调用 fork(2) 之前只有父进程 (PID:6651), num = 10
	I am parent process (PID:6651, child process PID:6703) num = 5
	I am child process (PID:6703, PPID:6651) num = 15
	PID:6703 Game Over num = 15
	PID:6651 Game Over num = 5


	写时复制(COW: Copy-on-Write)机制 :
	fork()刚创建子进程时，父子进程共享同一份物理内存页
	只有当某个进程要修改num时，内核才为该进程复制一份新的物理内存页
	这就是为什么父子进程的num值会不同


	执行流程可视化 :
	时间线:
	父进程(PID:6651)          子进程(PID:6703)
		|
		| num=10
		|
		+-- fork() ----------> 创建子进程
		|                      |
		| num=10               | num=10 (共享内存)
		|                      |
		| num-=5               | num+=5
		| num=5                | num=15
		|                      |
		| wait() --------------+ (等待子进程)
		|                      |
		|                      | 输出: "I am child..."
		|                      |
		|                      | 输出: "Game Over num=15"
		|                      |
		|                      | exit()
		|                      |
		| (子进程结束，回收)   |
		|                      |
		| 输出: "Game Over num=5"
		|                      |
		exit()
*/
