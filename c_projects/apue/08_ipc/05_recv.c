#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

#include "05_protocol.h"

int main(void)
{
	// 通过文件路径名和项目 ID 生成一个唯一的 key 值
	key_t key = ftok(PATH, PROJ_ID);
	if (key == -1)
	{
		perror("ftok()");
		return -1;
	}

	// 创建或获取消息队列，IPC_CREAT | IPC_EXCL 表示不存在则创建, 存在则失败
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
			return -2;
		}
	}


	struct msg_st recv_buf;  // 定义接收消息的结构体变量
	ssize_t count = 0;  // 存储实际接收到的消息正文字节数

	while (1)  // 循环接收消息, 直到出错或被信号中断
	{
		// 将整个结构体清零 (mtype 和 str), 确保 str 有结束符 '\0' 且无残留数据
		memset(&recv_buf, 0, sizeof(recv_buf));

		// 从消息队列接收消息
		// 消息正文最大长度 SIR_SIZE
		// msgtyp 消息类型传 0 (表示接收任意类型)
		// msgflg 传 0 (表示阻塞等待, 直到有消息到达)
		count = msgrcv(msg_id, &recv_buf, MSG_SIZE, 0, 0);  // 接收消息队列的数据
		if (count == -1)  // 接收失败
		{
			perror("msgrcv()");
			msgctl(msg_id, IPC_RMID, NULL);  // 销毁消息队列
			return -3;
		}
		puts(recv_buf.str);  // 将接收到的消息正文打印到标准输出 (此时 recv_buf.str 已由 memset 保证以 '\0' 结尾)
	}

	// 注意: 正常退出时未销毁队列, 如需清理可在此处调用 msgctl(msg_id, IPC_RMID, NULL)
	// 但本程序为无限循环, 通常需要外部中断 (如 Ctrl+C) 终止, 此时队列可能残留
	// 建议使用 ipcrm 命令手动删除, 或注册信号处理函数在退出时销毁

	return 0;
}