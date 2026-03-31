/*
	消息队列实现有亲缘关系的进程间通信 (带同步)

	子进程发送消息, 父进程成功接收消息后发送 ACK, 子进程接收到 ACK 才会继续发消息
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>
#include "08_homework_protocol.h"

#define MSG_COUNT 5  // 通信次数

int main(void)
{
	int ret = 0;  // 返回状态码

	// 通过文件路径名和项目 ID 生成一个唯一的 key 值, 用于标识消息队列
	key_t key = ftok(PATH, PROJ_ID);
	if (key == -1)
	{
		perror("ftok()");
		ret = -1;
		goto ERR_1;
	}

	pid_t pid = fork();  // 创建子进程
	if(pid == -1)
	{
		perror("fork()");
		ret = -2;
		goto ERR_1;
	}

	int msg_id = msgget(key, IPC_CREAT | IPC_EXCL | 0600);  // 创建 / 获取消息队列
	if (msg_id == -1)  // 创建失败
	{
		if (errno == EEXIST)  // 错误码为 EEXIST, 说明消息队列已存在
		{
			msg_id = msgget(key, 0);  // 直接获取已存在的消息队列标识符 (不创建, msgflg 传 0)
		}
		else  // 其他错误 (权限不足, 内存不足等)
		{
			perror("msgget()");
			ret = -3;
			goto ERR_1;
		}
	}

	if(pid == 0)  // 子进程
	{
		// ========== 子进程 : 发送消息给父进程 ==========
		
		struct msg_st send_msg;  // 存储待发送的消息
		struct msg_st ack_msg;   // 用于父进程是否接收确认


		for (int i = 1; i <= MSG_COUNT; i++)  // 循环发 MSG_COUNT 次消息
		{
			memset(&send_msg, 0, sizeof(send_msg));  // 每次发之前清一下
			
			send_msg.msg_type = MSG_TYPE_CHILD;  // 使用子进程的消息类型
			snprintf(send_msg.msg, MSG_SIZE, "子进程(PID=%d)发送第 %d 条消息", getpid(), i);

			// 发送消息到消息队列
			// strlen(send_buf.str) + 1 消息正文实际长度 (加一个结束符 '\0')
			// msgflg 传 0 表示阻塞发送
			if (msgsnd(msg_id, &send_msg, strlen(send_msg.msg) + 1, 0) == -1)
			{
				perror("child msgsnd()");
				exit(1);  // 打印错误, 终止子进程
			}
			printf("[子进程] 已经发送: %s\n", send_msg.msg);

			// 子进程等待接收父进程的 ACK, 接收再发送
			memset(&ack_msg, 0, sizeof(ack_msg));  // 每次发之前清一下
			// 只接收类型为 MSG_TYPE_PARENT 的消息
			if (msgrcv(msg_id, &ack_msg, MSG_SIZE, MSG_TYPE_PARENT, 0) == -1)
			{
				perror("child msgrcv()");
				exit(2);  // 打印错误, 终止子进程
			}
		}
		
		printf("[子进程] 所有消息发送完毕，退出\n");
		exit(0);  // 正常终止子程序
	}
	else  // 父进程
	{
		// ========== 父进程 : 接收子进程的消息 ==========

		struct msg_st recv_msg;  // 存储待接收的消息
		struct msg_st ack_msg;   // 用于子进程是否发送确认

		printf("[父进程] 开始接收子进程消息...\n");

		for (int i = 1; i <= MSG_COUNT; i++)  // 循环接收 MSG_COUNT 次消息
		{
			memset(&recv_msg, 0, sizeof(recv_msg));  // 每次接收之前清一下

			// 从消息队列接收消息
			// 消息正文最大长度 SIR_SIZE
			// 只接收类型为 MSG_TYPE_CHILD 的消息
			if (msgrcv(msg_id, &recv_msg, MSG_SIZE, MSG_TYPE_CHILD, 0) == -1)
			{
				perror("parent msgrcv()");
				ret = -5;
				goto ERR_2;
			}

			printf("[父进程] 收到消息: %s\n", recv_msg.msg);

			// 父进程发送 ACK 给子进程
			ack_msg.msg_type = MSG_TYPE_PARENT;
			snprintf(ack_msg.msg, MSG_SIZE, "父进程确认收到第%d条消息", i);
			if (msgsnd(msg_id, &ack_msg, strlen(ack_msg.msg) + 1, 0) == -1)
			{
				perror("parent msgsnd()");
				ret = -6;
				goto ERR_2;
			}
		}
		
		wait(NULL);  // 等待子进程结束
		printf("[父进程] 子进程已结束\n");
	}

ERR_2:
	msgctl(msg_id, IPC_RMID, NULL);  // 销毁消息队列

ERR_1:
	return ret;
}

/*
	[父进程] 开始接收子进程消息...
	[子进程] 已经发送: 子进程(PID=22407)发送第 1 条消息
	[父进程] 收到消息: 子进程(PID=22407)发送第 1 条消息
	[子进程] 已经发送: 子进程(PID=22407)发送第 2 条消息
	[父进程] 收到消息: 子进程(PID=22407)发送第 2 条消息
	[子进程] 已经发送: 子进程(PID=22407)发送第 3 条消息
	[父进程] 收到消息: 子进程(PID=22407)发送第 3 条消息
	[子进程] 已经发送: 子进程(PID=22407)发送第 4 条消息
	[父进程] 收到消息: 子进程(PID=22407)发送第 4 条消息
	[子进程] 已经发送: 子进程(PID=22407)发送第 5 条消息
	[父进程] 收到消息: 子进程(PID=22407)发送第 5 条消息
	[子进程] 所有消息发送完毕，退出
	[父进程] 子进程已结束
*/