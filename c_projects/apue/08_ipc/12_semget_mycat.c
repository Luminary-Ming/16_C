/*
	使用系统调用 I/O 函数实现 cat 命令
	使用 System V 信号量实现互斥, 确保每次只有一个进程读取文件
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define BUF_SIZE 8  // 定义缓冲区大小, 每次读取 8 字节

union semun  // System V 信号量初始化所需的共用体 
{
	int val;  // 用于设置信号量的初始值
};

// 子进程的工作函数, 循环从共享内存中获取任务
void work(int fd, int sem_id);

int main(int argc, char *argv[])
{
	// 检查命令行参数
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s + filename\n", argv[0]);
		return -1;
	}

	// 以只读方式打开文件
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("open()");
		return -2;
	}

	// 创建信号集, 包含 1 个信号量, 权限 0600
	int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
	if (sem_id == -1)  // 创建失败
	{
		perror("semget()");
		close(fd);  // 关闭文件
		return -3;
	}

	// 定义共用体, 将信号量初始值设为 1 (互斥锁)
	union semun arg;
	arg.val = 1;

	// 设置下标为 0 的信号量的值为 1
	if (semctl(sem_id, 0, SETVAL, arg) == -1)
	{
		perror("semctl()");
		close(fd);  // 关闭文件
		semctl(sem_id, 0, IPC_RMID);  // 销毁刚才创建的信号量集
		return -4;
	}

	// 创建 4 个子进程
	for (int i = 0; i < 4; i++)
	{
		pid_t pid = fork();  // 创建子进程
		if (pid == -1)
		{
			perror("fork()");
			close(fd);  // 关闭文件
			semctl(sem_id, 0, IPC_RMID);  // 销毁信号量集
			exit(1);  // 终止子进程, 状态码为 1
		}

		if(pid == 0)  // 子进程
		{
			work(i);  // 工作任务
			exit(0);  // 任务完成, 正常终止子进程
		}
	}

	// 父进程等待所有子进程结束
	for (int i = 0; i < 4; i++)
		wait(NULL);

	// 释放资源
	close(fd);  // 关闭文件
	semctl(sem_id, 0, IPC_RMID);  // 销毁信号量集

	return 0;  // 程序正常结束
}

// 子进程的工作函数, 循环从共享内存中获取任务
void work(int fd, int sem_id)
{
	// P 操作结构体
	// 对第 0 个信号量减 1, SEM_UNDO 表示进程异常退出时自动恢复信号量, 避免死锁
	struct sembuf p_op = { 0, -1, SEM_UNDO };
	// V 操作结构体
	// 对第 0 个信号量加 1, SEM_UNDO 表示进程异常退出时自动恢复信号量, 避免死锁
	struct sembuf v_op = { 0, 1, SEM_UNDO };

	while (1)  // 无限循环，直到读取完整个文件
	{
		// 加锁 (P 操作), 互斥访问文件
		if (semop(sem_id, &p_op, 1) == -1)
		{
			perror("semop(P)");
			exit(2);
		}

		char buf[BUF_SIZE];  // 缓冲区
		ssize_t count = read(fd, buf, BUF_SIZE);  // 从 fd 中最多读取 BUF_SIZE 字节到 buf 中
		if (count == -1)
		{
			perror("read()");
			semop(sem_id, &v_op, 1);  // 出错解锁 (V 操作), 避免死锁
			exit(3);
		}
		else if (count == 0)  // 如果读到的字节数为 0, 说明到达文件末尾
		{
			semop(sem_id, &v_op, 1);  // 解锁 (V 操作)
			break;  // 读完了退出循环
		}

		sleep(1);  // 模拟处理耗时 (演示并发效果), 实际可去掉 sleep

		write(STDOUT_FILENO, buf, count);  // 从 buf 中读取 count 字节数据到 stdout 中
	
		// 解锁, 允许其他进程读取
		if (semop(sem_id, &v_op, 1) == -1)
		{
			perror("semop(V)");  // 操作失败时打印错误
			exit(4);  // 子进程退出
		}
	}
}
