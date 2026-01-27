#ifndef __UTILS_H
#define __UTILS_H

#include "contact.h"

// 错误码定义
typedef enum
{
	SUCCESS = 0,  // 操作成功
	ERROR_MYSQL = -1,  // mysql 错误
	ERROR_CONTACT_NOT_FOUND = -2,  // 联系人没有找到
	ERROR_INVALID_INPUT = -3,  // 无效输入
	ERROR_SYSTEM = -4,  // 系统错误
}ErrorCode;

// JSON 响应结构体
typedef struct
{ 
	int code;  // 响应状态码
	char message[100];  // 响应消息
	char data[4096];  // 响应数据
}JsonResponse;


// 函数声明
void json_response_init(JsonResponse *resp, int code, const char *message);
void json_response_set_data(JsonResponse *resp, const char *data);
char *json_response_to_string(JsonResponse *resp);

char *contacts_to_json(Contact **contacts, int count);
char *contact_to_json(Contact *contact);

#endif