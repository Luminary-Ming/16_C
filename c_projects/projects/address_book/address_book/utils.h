#ifndef __UTILS_H
#define __UTILS_H

#include "contact.h"
#include <stddef.h>  // 添加这一行，用于 size_t 类型
#include <stdint.h>

// 错误码定义
typedef enum
{
	SUCCESS = 200,  // 操作成功
	ERROR_MYSQL = 501,  // mysql 错误
	ERROR_CONTACT_NOT_FOUND = 502,  // 联系人没有找到
	ERROR_INVALID_INPUT = 503,  // 无效输入
	ERROR_SYSTEM = 0,  // 系统错误
}ErrorCode;

// JSON 响应结构体
typedef struct
{ 
	int code;  // 响应状态码
	char message[100];  // 响应消息
	char data[4096];  // 响应数据
}JsonResponse;

void json_response_init(JsonResponse *resp, int code, const char *message);  // 初始化 JSON 响应
void json_response_set_data(JsonResponse *resp, const char *data);  // 设置 JSON 响应数据
char *json_response_to_string(JsonResponse *resp);  // JSON 响应转字符串

char *contacts_to_json(Contact **contacts, int count);  // 单个联系人转 JSON
char *contact_to_json(Contact *contact);  // 联系人列表转JSON

char *upload_image(const char *object_name);  // 上传图片, 返回 URL


// 在 utils.h 的末尾添加
unsigned char *base64_decode(const char *input, size_t *output_length);

#endif