/*
	工具函数
*/
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "minio_server.h"
#include <time.h>

// 初始化JSON响应
void json_response_init(JsonResponse *resp, int code, const char *message)
{
    if (!resp) return;

    resp->code = code;
    if (message)
        strncpy(resp->message, message, sizeof(resp->message) - 1);
    else
        resp->message[0] = '\0';
    resp->data[0] = '\0';
}

// 设置JSON响应数据
void json_response_set_data(JsonResponse *resp, const char *data)
{
    if (!resp || !data) return;

    strncpy(resp->data, data, sizeof(resp->data) - 1);
    resp->data[sizeof(resp->data) - 1] = '\0';
}

// JSON响应转字符串
char *json_response_to_string(JsonResponse *resp)
{
    if (!resp) return NULL;

    // 计算所需空间 ( 计算格式化字符串所需缓冲区大小)
    // 当第一个参数是 NULL，第二个参数是 0 时
    // snprintf 不实际写入任何内容
    // 但它仍然计算格式化后的字符串长度
    // 返回的是理论上需要的缓冲区大小
    size_t needed = snprintf(NULL, 0,
        "{\"code\":%d,\"message\":\"%s\",\"data\":%s}",
        resp->code, resp->message,
        resp->data[0] ? resp->data : "null");

    needed += 10;  // 安全边界

    char *json = malloc(needed);  // 分配足够空间
    if (!json) return NULL;

    snprintf(json, needed,
        "{\"code\": %d, \"message\": \"%s\", \"data\": %s}",
        resp->code, resp->message,
        resp->data[0] ? resp->data : "null");

    return json;
}

// 单个联系人转JSON
char *contact_to_json(Contact *contact)
{
    if (!contact) return NULL;

    // 分配足够大的缓冲区
    size_t buffer_size = 1024; // 增加缓冲区大小
    char *json = malloc(buffer_size);
    if (!json) return NULL;

    int len = snprintf(json, buffer_size,
        "{\"id\": %d, \"name\": \"%s\", \"telephone\": \"%s\", "
        "\"email\": \"%s\", \"initial\": \"%c\", \"image\": \"%s\", \"del\": %d}",
        contact->id, contact->name, contact->telephone,
        contact->email, contact->initial, contact->image, contact->del);

    // 检查是否截断
    if (len >= buffer_size)
    {
        // 如果缓冲区不足，重新分配更大的
        free(json);
        buffer_size = len + 1;
        json = malloc(buffer_size);
        if (!json) return NULL;

        snprintf(json, buffer_size,
            "{\"id\": %d, \"name\": \"%s\", \"telephone\": \"%s\", "
            "\"email\": \"%s\", \"initial\": \"%c\", \"image\": \"%s\", \"del\": %d}",
            contact->id, contact->name, contact->telephone,
            contact->email, contact->initial, contact->image, contact->del);
    }

    return json;
}

// 联系人列表转JSON
char *contacts_to_json(Contact **contacts, int count)
{
    if (!contacts || count <= 0)
    {
        char *empty = malloc(3);
        if (empty)
            strcpy(empty, "[]");
        return empty;
    }


    // 为每个联系人预先计算JSON字符串
    char **contact_jsons = malloc(count * sizeof(char *));
    if (!contact_jsons) return NULL;

    // 计算所需空间
    size_t total_size = 3;  // "[]"
    int valid_count = 0;    // 实际有效的联系人数量
    for (int i = 0; i < count; i++)
    {
        if (contacts[i])
        {
            contact_jsons[valid_count] = contact_to_json(contacts[i]);
            if (contact_jsons[valid_count])
            {
                total_size += strlen(contact_jsons[valid_count]) + 1; // +1 逗号
                valid_count++;
            }
        }
    }

    if (valid_count == 0)
    {
        free(contact_jsons);
        char *empty = malloc(3);
        if (empty) strcpy(empty, "[]");
        return empty;
    }

    char *json = malloc(total_size);  // 分配足够的内存
    if (!json)
    {
        for (int i = 0; i < valid_count; i++)
        {
            free(contact_jsons[i]);
        }
        free(contact_jsons);
        return NULL;
    }

    strcpy(json, "[");

    for (int i = 0; i < valid_count; i++)
    {
        if (i > 0)
        {
            strcat(json, ",");
        }
        strcat(json, contact_jsons[i]);
        free(contact_jsons[i]);  // 释放每个联系人的JSON
    }

    strcat(json, "]");

    free(contact_jsons);
    return json;
}

// 处理文件名
char *handle_filename(const char *object_name)
{
    if (!object_name || strlen(object_name) == 0)
        return  NULL; 

    // 获取当前时间戳(秒数)
    // time_t timestamp = time(NULL);
    struct timeval tv;  // 时间结构体(秒, 微秒)
    gettimeofday(&tv, NULL);  // 获取当前时间, 赋值给 tv 结构体
    long timestamp = (long)tv.tv_sec;

    // 文件名拼上时间戳
    const char *dot = strrchr(object_name, '.');  // 文件名 '.' 的位置

    char prefix[128] = { 0 };  // 文件名前缀
    int len = dot - object_name;  // 前缀长度
    strncpy(prefix, object_name, len);
    prefix[len] = '\0';

    char *new_name = malloc(256);  // 存储新名字
    if (!new_name) return NULL;
    snprintf(new_name, 256, "%s_%ld.%s", prefix, timestamp, dot + 1);

    return new_name;
}


unsigned char *base64_decode(const char *input, size_t *output_length)
{
    const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int input_len = strlen(input);

    if (input_len % 4 != 0) return NULL;

    size_t output_len = input_len / 4 * 3;
    if (input[input_len - 1] == '=') output_len--;
    if (input[input_len - 2] == '=') output_len--;

    unsigned char *output = malloc(output_len);
    if (!output) return NULL;

    for (int i = 0, j = 0; i < input_len;)
    {
        // 获取4个字符的索引
        uint32_t sextet_a = 0, sextet_b = 0, sextet_c = 0, sextet_d = 0;

        const char *pos_a = strchr(base64_table, input[i]);
        if (pos_a && input[i] != '=')
        {
            sextet_a = pos_a - base64_table;
        }
        i++;

        if (i < input_len)
        {
            const char *pos_b = strchr(base64_table, input[i]);
            if (pos_b && input[i] != '=')
            {
                sextet_b = pos_b - base64_table;
            }
            i++;
        }

        if (i < input_len)
        {
            const char *pos_c = strchr(base64_table, input[i]);
            if (pos_c && input[i] != '=')
            {
                sextet_c = pos_c - base64_table;
            }
            i++;
        }

        if (i < input_len)
        {
            const char *pos_d = strchr(base64_table, input[i]);
            if (pos_d && input[i] != '=')
            {
                sextet_d = pos_d - base64_table;
            }
            i++;
        }

        // 组合成24位
        uint32_t triple = (sextet_a << 18) | (sextet_b << 12) | (sextet_c << 6) | sextet_d;

        // 分解为3个字节
        if (j < output_len) output[j++] = (triple >> 16) & 0xFF;
        if (j < output_len) output[j++] = (triple >> 8) & 0xFF;
        if (j < output_len) output[j++] = triple & 0xFF;
    }

    *output_length = output_len;
    return output;
}