/*
	共享内存实现进程间通信 (非亲缘关系) - 写入端

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

	printf("[写入端] 生成 key: %d\n", key);


	// 创建共享内存
	// 0600 只有所有者可读可写, 共享内存大小 4096 字节
	int shm_id = shmget(key, 4096, IPC_CREAT | IPC_EXCL | 0600);
	if (shm_id == -1)  // 创建失败
	{
		if (errno == EEXIST)  // 错误码为 EEXIST 说明共享内存已存在
		{
			// 获取已存在的共享内存, 不创建 (shmflg = 0)
			shm_id = shmget(key, 4096, 0);

			printf("[写入端] 获取已存在的共享内存, shm_id=%d\n", shm_id);
		}
		else  // 其他错误 (权限不足, 内存不足等)
		{
			perror("shmget()");
			ret = -2;
			goto ERR_1;
		}
	}
	else  // 创建成功
		printf("[写入端] 创建新的共享内存, shm_id=%d\n", shm_id);

	// 将共享内存映射到当前进程的虚拟地址空间
	void *ptr = shmat(shm_id, NULL, 0);
	if (ptr == (void *)-1)  // 映射失败
	{
		perror("shmat()");
		ret = -3;
		goto ERR_2;
	}

	printf("[写入端] 共享内存映射成功, 地址=%p\n", ptr);

	// 写入数据到共享内存
	printf("[写入端] 请输入要写入的数据 (不超过%d字节): ", SHM_SIZE - 1);
	char write_buf[SHM_SIZE];
	fgets(write_buf, SHM_SIZE, stdin);
	write_buf[strcspn(write_buf, "\n")] = '\0';  // 去除换行符

	memset(ptr, 0, SHM_SIZE);  // 清空共享内存
	strncpy(ptr, write_buf, SHM_SIZE - 1);  // 复制数据
	printf("[写入端] 数据已写入共享内存: %s\n", write_buf);

	// 等待用户输入, 再删除共享内存
	printf("[写入端] 按回车键删除共享内存...");
	getchar();

	shmdt(ptr);  // 解除共享内存映射

	printf("[写入端] 解除共享内存映射\n");

ERR_2:
	// shmctl 控制共享内存; IPC_RMID 立即删除共享内存 (即使还有进程映射)
	shmctl(shm_id, IPC_RMID, NULL);  // 销毁共享内存

ERR_1:
	return ret;
}