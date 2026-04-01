#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define MIN 100  // 待检测数字范围的最小值
#define MAX 300  // 待检测数字范围的最大值
#define LENGTH 4096  // 共享内存大小
#define CHILD_NUM 4  // 子进程数量 (并发数)

// semctl(2) 系统调用在初始化信号量时需要的联合体
union semun
{
	int val;  // 信号量初始值
};

//  全局变量 (父子进程共享, 通过 fork() 继承, 指向同一块物理内存)
int *shm_ptr = NULL;  // 全局变量, 指向共享内存的指针, 存储当前要检测的数字
int semid;  // 全局变量, 信号量标识符, 用于互斥访问共享内存

// 初始化信号量为 1 (用作互斥锁)
void sem_init(int semid)
{
	union semun tmp;  // 创建联合体变量

	tmp.val = 1;  // 设置初始值为 1 (未锁定状态)

	// SETVAL 设置信号量的值
	// 0 表示操作第 0 个信号量
	semctl(semid, 0, SETVAL, tmp);  // 对信号量数组下标为 0 的信号量设置为 1
}

// P 操作 -> 加锁 / 申请资源 (信号量 - 1, 如果信号量为 0 则阻塞等待)
void sem_p(int semid)
{
	struct sembuf buf;  // 信号量操作结构体

	buf.sem_num = 0;  // 操作第 0 个信号量
	buf.sem_op = -1;  // P 操作, 信号量 - 1
	buf.sem_flg = SEM_UNDO;  // 进程异常退出时自动恢复信号量, 避免死锁

	semop(semid, &buf, 1);  // 执行信号量操作
}

// V 操作 -> 解锁 / 释放资源 (信号量 + 1, 唤醒等待的进程)
void sem_v(int semid)
{
	struct sembuf buf;  // 信号量操作结构体

	buf.sem_num = 0;  // 操作第 0 个信号量
	buf.sem_op = 1;  // V 操作, 信号量 + 1
	buf.sem_flg = SEM_UNDO;  // 进程异常退出时自动恢复信号量, 避免死锁

	semop(semid, &buf, 1);  // 执行信号量操作
}

// 判断质数
static int is_prime(int n)
{
	if (n <= 1) return 0;
	if (n == 2) return 1;  // 2 是最小的质数
	if (n % 2 == 0) return 0;  // // 偶数都不是质数

	for (int i = 3; i * i <= n; i += 2)  // 只检查奇数因子，且只检查到平方根即可
	{
		if (n % i == 0)
			return 0;  // 能被整除, 不是质数
	}

	return 1;  // 是质数
}

// 子进程的工作函数, 循环从共享内存中获取任务 (数字), 判断是否为质数
void work(int n)  // n 作为子进程的编号
{
	int num = 0;  // 临时存储从共享内存读取的数字

	while (1)  // 持续工作直到所有任务完成
	{
		// ========== 临界区开始 ==========

		sem_p(semid);  // P 操作, 加锁, 互斥访问共享内存

		// 检查是否还有未处理的任务
		if (*shm_ptr > MAX)  // 如果数字大于 MAX, 任务结束
		{
			sem_v(semid);  // V 操作, 解锁
			break;  // 所有数字已处理完毕, 跳出死循环
		}

		// 获取当前任务数字, 并将共享内存中的值自增 (指向下一个任务)
		num = *shm_ptr;  // 读取当前要检测的数字
		(*shm_ptr)++;  // 更新为下一个待检测的数字

		sem_v(semid);  // V 操作, 解锁

		// ========== 临界区结束 ==========

		// 在临界区外进行质数判断 (耗时操作, 不占用锁)
		if (is_prime(num))
			printf("[%d] %d is a prime !\n", n, num);  //  输出结果: 子进程编号和质数
	}
}


int main(void)
{
	// ========== 1. 创建共享内存 ==========
	// 创建共享内存, 大小为 4byte, 存储一个整型数字
	int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);

	// 将共享内存映射到当前进程的虚拟地址空间
	// NULL 让系统自动选择地址; 0 读写权限
	shm_ptr = (int *)shmat(shmid, NULL, 0);
	
	// 初始化共享内存, 将最小值放入, 作为任务起始点
	*shm_ptr = MIN;
	

	// ========== 2. 创建信号量 (用于互斥) ==========
	// 创建信号量数组, 信号量的个数为 1
	// IPC_PRIVATE 创建私有信号量集; 1 包含 1 个信号量
	semid = semget(IPC_PRIVATE, 1, IPC_CREAT);

	sem_init(semid);  // 调用内部函数进行初始化


	// ========== 3. 创建子进程并发处理任务 ==========
	// 创建 CHILD_NUM 个子进程
	for (int n = 0; n < CHILD_NUM; n++)
	{
		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork()");
			exit(1);  // 由于创建子进程失败, 终止进程, 并且返回状态 1
		}

		if (pid == 0)  // 子进程
		{
			work(n);  // 执行工作任务
			exit(0);  // 任务完成, 正常终止子进程
		}
		// 父进程继续循环创建下一个子进程
	}

	// ========== 4. 父进程等待所有子进程结束 ==========
	// 父进程的操作
	for (int i = 0; i < CHILD_NUM; i++)
		wait(NULL);  // 等待子进程结束

	// ========== 5. 释放资源 ==========
	shmdt(shm_ptr);  // 解除共享内存映射
	shmctl(shmid, IPC_RMID, NULL);  // 销毁共享内存
	semctl(semid, 0, IPC_RMID);  // 销毁信号量

	return 0;
}