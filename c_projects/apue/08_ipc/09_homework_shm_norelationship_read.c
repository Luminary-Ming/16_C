/*
	共享内存实现进程间通信 (非亲缘关系) - 读取端

	创建一块 4096 字节的私有共享内存
	一个程序去往共享内存中写
	一个程序去往共享内存中读
	读完之后删除共享内存
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

#define SHM_PATH "/tmp/shm_key"  // 用于生成 key 的路径 (必须存在)
#define SHM_PROJ_ID 'A'  // 项目ID
#define SHM_SIZE 4096  // 共享内存大小

int main(void)
{
	int ret = 0;  // 返回状态码

	// 通过文件路径名和项目 ID 生成一个唯一的 key 值, 用于标识消息队列
	key_t key = ftok(SHM_PATH, SHM_PROJ_ID);
	if (key == -1)
	{
		perror("ftok()");
		ret = -1;
		goto ERR_1;
	}

	printf("[读取端] 生成key: %d\n", key);


	// 读取端获取已存在的共享内存 (shmflg=0 表示只获取不创建)
	int shm_id = shmget(key, SHM_SIZE, 0);
	if (shm_id == -1)  // 创建失败
	{
		perror("shmget()");
		ret = -2;
		goto ERR_1;
	}
	else  // 获取成功
		printf("[读取端] 获取共享内存成功, shm_id=%d\n", shm_id);

	// 将共享内存映射到当前进程的虚拟地址空间
	void *ptr = shmat(shm_id, NULL, 0);
	if (ptr == (void *)-1)  // 映射失败
	{
		perror("shmat()");
		ret = -3;
		goto ERR_2;
	}

	printf("[读取端] 共享内存映射成功, 地址=%p\n", ptr);

	printf("[读取端] 读取共享内存数据: %s\n", (char *)ptr);

	shmdt(ptr);  // 解除共享内存映射

	printf("[读取端] 解除共享内存映射\n");

ERR_2:
	// 读取端不删除共享内存, 由写入端负责删除

ERR_1:
	return ret;
}
