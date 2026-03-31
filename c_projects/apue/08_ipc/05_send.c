#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "05_protocol.h"

int main(int argc, char *argv[])
{
	// 检查命令行参数数量, 至少需要 3 个参数 (程序名、消息类型、消息正文)
	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s + id + msg\n", argv[0]);
		return -1;
	}


	// 通过文件路径名和项目 ID 生成一个唯一的 key 值, 用于标识消息队列
	key_t key = ftok(PATH, PROJ_ID);
	if (key == -1)
	{
		perror("ftok()");
		return -2;
	}

	// 创建消息队列，IPC_CREAT | IPC_EXCL 表示不存在则创建, 存在则失败
	// 0600 表示所有者可读可写, 其他用户无权限
	int msg_id = msgget(key, IPC_CREAT | IPC_EXCL | 0600);
	if (msg_id == -1)  // 创建消息队列失败
	{
		if (errno == EEXIST)  // 错误码为 EEXIST, 说明消息队列已存在
		{
			msg_id = msgget(key, 0);  // 直接获取已存在的消息队列标识符 (不创建, msgflg 传 0)
		}
		else  // 其他错误 (如权限不足、内存不足等)
		{
			perror("msgget()");
			return -3;
		}
	}

	// 发送数据
	struct msg_st send_buf;  // 存储待发送的消息
	memset(&send_buf, 0, sizeof(send_buf));  // 将整个结构体清零 (mtype 和 str)

	// 将命令行第一个参数 (消息类型) 转换为整数, 赋值给 mtype
	send_buf.mtype = atoi(argv[1]);
	
	// 将命令行第二个参数 (消息正文) 复制到 str 中, 最多复制 SIR_SIZE-1 个字符
	// 留出一个字节用于存放字符串结束符 '\0'
	strncpy(send_buf.str, argv[2], MSG_SIZE - 1);
	// 手动添加字符串结束符, 确保 str 始终以 '\0' 结尾 (如果 src 长度大于 n, strncpy 只会复制前 n 个字符，不会在末尾添加 '\0')
	send_buf.str[MSG_SIZE - 1] = '\0';
	
	// 发送消息到消息队列
	// strlen(send_buf.str) + 1 消息正文实际长度 (加一个结束符 '\0')
	// msgflg 传 0 表示阻塞发送
	int ret = msgsnd(msg_id, &send_buf, strlen(send_buf.str) + 1, 0);
	if (ret == -1)  // 发送失败
	{
		perror("msgsnd()");
		msgctl(msg_id, IPC_RMID, NULL);  // 销毁消息队列
		return -4;
	}

	// 可选: 发送成功后也可以不销毁队列, 由接收端负责销毁
	// 本例中未主动销毁, 接收端在出错时会销毁, 正常运行时需手动删除或依赖 ipcrm 命令

	return 0;
}