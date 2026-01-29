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

    char *json = malloc(5000);  // 分配足够空间
    if (!json) return NULL;

    snprintf(json, 5000,
        "{\"code\": %d, \"message\": \"%s\", \"data\": %s}",
        resp->code, resp->message,
        resp->data[0] ? resp->data : "null");

    return json;
}

// 单个联系人转JSON
char *contact_to_json(Contact *contact)
{
    if (!contact) return NULL;

    char *json = malloc(1024);
    if (!json) return NULL;

    snprintf(json, 1024,
        "{\"id\": %d, \"name\": \"%s\", \"telephone\": \"%s\", "
        "\"email\": \"%s\", \"initial\": \"%c\", \"image\": \"%s\", \"del\": %d}",
        contact->id, contact->name, contact->telephone,
        contact->email, contact->initial, contact->image, contact->del);

    return json;
}

// 联系人列表转JSON
char *contacts_to_json(Contact **contacts, int count)
{
    if (!contacts || count <= 0)
    {
        char *empty = malloc(3);
        if (empty) strcpy(empty, "[]");
        return empty;
    }

    // 计算所需空间
    size_t total_size = 3;  // "[]"
    for (int i = 0; i < count; i++)
    {
        if (contacts[i])
        {
            // 估计每个联系人JSON的大小
            total_size += 300;
        }
    }

    char *json = malloc(total_size);
    if (!json) return NULL;

    strcpy(json, "[");

    for (int i = 0; i < count; i++)
    {
        if (contacts[i])
        {
            char *contact_json = contact_to_json(contacts[i]);
            if (contact_json)
            {
                if (i > 0) strcat(json, ",");
                strcat(json, contact_json);
                free(contact_json);
            }
        }
    }

    strcat(json, "]");
    return json;
}

// 上传文件
char *upload_image(const char *object_name)
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

    char new_name[256] = { 0 };  // 存储新名字
    snprintf(new_name, 256, "%s_%ld.%s", prefix, timestamp, dot + 1);

    // 创建 MinIO 配置
    MinioConfig *config = minio_create();
    if (!config)
    {
        fprintf(stderr, "Failed to create MinIO config\n");
        return NULL;
    }

    // 上传文件到 MinIO
    int result = minio_upload(config, new_name, object_name);
    if (result != 0)
    {
        fprintf(stderr, "Failed to upload to MinIO: %d\n", result);
        minio_destroy(config);
        return NULL;
    }

    // 生成预览 URL
    char *url = minio_preview_url(config, new_name);

    minio_destroy(config);

    return url;
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