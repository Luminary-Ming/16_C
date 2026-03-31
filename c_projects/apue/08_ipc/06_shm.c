/*
	共享内存

	创建一块 1024 字节的私有共享内存
	子进程将数据 "hello world!" 写入共享内存后退出
	父进程等待子进程结束，然后读取并打印共享内存中的内容
	删除共享内存
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int ret = 0;  // 返回状态码

	// 创建共享内存
	// IPC_PRIVATE 生成唯一 key; IPC_CREAT | IPC_EXCL 创建新共享内存, 不存在才创建
	// 0600 只有所有者可读可写, 共享内存大小 1024 字节
	int shm_id = shmget(IPC_PRIVATE, 1024, IPC_CREAT | IPC_EXCL | 0600);
	if (shm_id == -1)  // 创建失败
	{
		if (errno == EEXIST)  // 错误码为 EEXIST 说明共享内存已存在
		{
			// 获取已存在的共享内存, 不创建 (shmflg = 0)
			shm_id = shmget(IPC_PRIVATE, 1024, 0);
		}
		else  // 其他错误 (权限不足, 内存不足等)
		{
			perror("shmget()");
			ret = -1;
			goto ERR_1;
		}
	}

	// 创建子进程
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork()");
		ret = -2;
		goto ERR_2;
	}

	// 父子进程分别将共享内存映射到虚拟地址空间
	if (pid == 0)  // 子进程
	{
		void *ptr = shmat(shm_id, NULL, 0);  // 映射共享内存
		if (ptr == (void *)-1)  // 映射失败
		{
			perror("shmat()");
			exit(1);  // 打印错误, 终止子进程
		}
		memcpy(ptr, "hello world!", 12);  // 将字符串复制到共享内存
		shmdt(ptr);  // 解除共享内存与当前进程的映射 (没有销毁内存)
		exit(0);  // 子进程正常终止
	}
	else  // 父进程
	{
		wait(NULL);  // 父进程阻塞, 等待任意子进程结束 (NULL 表示不关心子进程退出状态)

		void *ptr = shmat(shm_id, NULL, 0);  // 映射共享内存
		if (ptr == (void *)-1)  // 映射失败
		{
			perror("shmat()");
			ret = -3;
			goto ERR_2;
		}
		puts((char *)ptr);  // 将共享内存内容作为字符串打印 (puts 自动换行)
		shmdt(ptr);  // 父进程解除共享内存映射
	}

ERR_2:
	// shmctl 控制共享内存; IPC_RMID 立即删除共享内存 (即使还有进程映射)
	shmctl(shm_id, IPC_RMID, NULL);  // 销毁共享内存

ERR_1:
	return ret;
}