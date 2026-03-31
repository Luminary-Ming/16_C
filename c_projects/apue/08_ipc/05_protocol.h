#ifndef PROTOCOL_H
#define PROTOCOL_H

#define PATH "/etc/passwd"  // 规定好使用同一个 PATH, 作为 ftok() 的参数, 生成唯一的 key
#define PROJ_ID 'a'  // 规定好使用同一个 PROJ_ID, 配合 PATH 生成 key

#define MSG_SIZE 1024  // 规定好消息正文的最大长度 (不超过 1024 byte)

// 消息队列对话结构体
struct msg_st
{
	long mtype;  // 消息类型标签 (正整数), 用于接收端按类型过滤或实现优先级 (msgrcv 的 msgtyp 参数)
	char str[MSG_SIZE];  // 消息正文缓冲区, 存储实际数据 (可为文本或二进制)
};

#endif
