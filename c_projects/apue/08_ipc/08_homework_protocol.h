#ifndef HOMEWORK_PROTOCOL_H
#define HOMEWORK_PROTOCOL_H

#define PATH "/tmp/msg_queue"
#define PROJ_ID 'A'

#define MSG_SIZE 4096
#define MSG_TYPE_PARENT 10  // 父进程的消息类型
#define MSG_TYPE_CHILD 20   // 子进程的消息类型

struct msg_st
{
	long msg_type;  // 消息类型
	char msg[MSG_SIZE];  // 消息正文
};

#endif
