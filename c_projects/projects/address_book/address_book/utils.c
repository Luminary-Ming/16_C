/*
	工具函数
*/
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        "\"email\": \"%s\", \"initial\": \"%c\", \"del\": %d}",
        contact->id, contact->name, contact->telephone,
        contact->email, contact->initial, contact->del);

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