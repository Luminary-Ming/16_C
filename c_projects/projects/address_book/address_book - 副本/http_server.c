#include "http_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>
#include "contact_llist.h"
#include "db_mysql.h"
#include "utils.h"
#include "contact.h"

// 全局链表
void *global_contact_list = NULL;

// HTTP服务器实例
static HttpServer server = { 0 };

// 发送JSON响应
static int send_json_response(struct MHD_Connection *connection,
    int status_code, const char *json)
{
    struct MHD_Response *response;
    int ret;

    response = MHD_create_response_from_buffer(strlen(json),
        (void *)json,
        MHD_RESPMEM_MUST_COPY);

    if (!response)
        return MHD_NO;

    MHD_add_response_header(response, "Content-Type", "application/json");
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
    MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

    ret = MHD_queue_response(connection, status_code, response);
    MHD_destroy_response(response);

    return ret;
}

// 处理OPTIONS请求（CORS预检）
static int handle_options(struct MHD_Connection *connection)
{
    struct MHD_Response *response;
    int ret;

    response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);

    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
    MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");
    MHD_add_response_header(response, "Access-Control-Max-Age", "86400");

    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

// 处理GET /api/contacts 获取所有联系人
static int handle_get_contacts(struct MHD_Connection *connection)
{
    JsonResponse resp;
    json_response_init(&resp, SUCCESS, "Success");

    if (!global_contact_list)
    {
        json_response_init(&resp, ERROR_SYSTEM, "Contact list not initialized");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, json);
        free(json);
        return ret;
    }

    // 重新从数据库加载
    if (!db_load_contacts(global_contact_list))
    {
        json_response_init(&resp, ERROR_MYSQL, "Failed to load contacts from database");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, json);
        free(json);
        return ret;
    }

    LLIST *handler = (LLIST *)global_contact_list;
    int count = llist_get_count(handler);

    // 收集所有联系人
    Contact **contacts = malloc(count * sizeof(Contact *));
    if (!contacts)
    {
        json_response_init(&resp, ERROR_SYSTEM, "Memory allocation failed");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, json);
        free(json);
        return ret;
    }

    int actual_count = 0;
    for (int i = 0; i < count; i++)
    {
        Contact *contact = llist_get_at(handler, i);
        if (contact && contact->del == 0)
        {
            contacts[actual_count++] = contact;
        }
    }

    char *data_json = contacts_to_json(contacts, actual_count);
    free(contacts);

    if (!data_json)
    {
        json_response_init(&resp, ERROR_SYSTEM, "Failed to generate JSON");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, json);
        free(json);
        return ret;
    }

    json_response_set_data(&resp, data_json);
    char *json = json_response_to_string(&resp);

    int ret = send_json_response(connection, MHD_HTTP_OK, json);

    free(data_json);
    free(json);

    return ret;
}

// 处理GET /api/contacts/{id} 获取单个联系人
static int handle_get_contact(struct MHD_Connection *connection, int id)
{
    JsonResponse resp;

    if (!global_contact_list)
    {
        json_response_init(&resp, ERROR_SYSTEM, "Contact list not initialized");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, json);
        free(json);
        return ret;
    }

    // 先从数据库查找
    Contact *contact = db_find_contact(id);
    if (!contact)
    {
        json_response_init(&resp, ERROR_CONTACT_NOT_FOUND, "Contact not found");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_NOT_FOUND, json);
        free(json);
        return ret;
    }

    char *contact_json = contact_to_json(contact);
    if (!contact_json)
    {
        free(contact);
        json_response_init(&resp, ERROR_SYSTEM, "Failed to generate JSON");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, json);
        free(json);
        return ret;
    }

    json_response_init(&resp, SUCCESS, "Success");
    json_response_set_data(&resp, contact_json);
    char *json = json_response_to_string(&resp);

    int ret = send_json_response(connection, MHD_HTTP_OK, json);

    free(contact_json);
    free(json);
    free(contact);

    return ret;
}

// 解析POST/PUT请求中的JSON数据
static Contact *parse_contact_from_json(const char *json_str)
{
    // 简化解析，实际项目应使用cJSON库
    Contact *contact = malloc(sizeof(Contact));
    if (!contact) return NULL;

    memset(contact, 0, sizeof(Contact));

    // 简单解析，查找字段
    const char *name_start = strstr(json_str, "\"name\":\"");
    if (name_start)
    {
        name_start += 8;
        const char *name_end = strchr(name_start, '"');
        if (name_end)
        {
            int len = name_end - name_start;
            if (len > NAMESIZE - 1) len = NAMESIZE - 1;
            strncpy(contact->name, name_start, len);
            contact->name[len] = '\0';
        }
    }

    const char *tel_start = strstr(json_str, "\"telephone\":\"");
    if (tel_start)
    {
        tel_start += 13;
        const char *tel_end = strchr(tel_start, '"');
        if (tel_end)
        {
            int len = tel_end - tel_start;
            if (len > TELEPHONESIZE - 1) len = TELEPHONESIZE - 1;
            strncpy(contact->telephone, tel_start, len);
            contact->telephone[len] = '\0';
        }
    }

    const char *email_start = strstr(json_str, "\"email\":\"");
    if (email_start)
    {
        email_start += 9;
        const char *email_end = strchr(email_start, '"');
        if (email_end)
        {
            int len = email_end - email_start;
            if (len > EMAILSIZE - 1) len = EMAILSIZE - 1;
            strncpy(contact->email, email_start, len);
            contact->email[len] = '\0';
        }
    }

    const char *id_start = strstr(json_str, "\"id\":");
    if (id_start)
    {
        id_start += 5;
        contact->id = atoi(id_start);
    }

    return contact;
}

// 处理POST /api/contacts 添加联系人
static int handle_post_contact(struct MHD_Connection *connection, const char *post_data)
{
    JsonResponse resp;

    if (!post_data || strlen(post_data) == 0)
    {
        json_response_init(&resp, ERROR_INVALID_INPUT, "Empty request body");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_BAD_REQUEST, json);
        free(json);
        return ret;
    }

    Contact *contact = parse_contact_from_json(post_data);
    if (!contact)
    {
        json_response_init(&resp, ERROR_INVALID_INPUT, "Failed to parse contact data");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_BAD_REQUEST, json);
        free(json);
        return ret;
    }


    // 添加到数据库
    if (!db_add_contact(contact))
    {
        free(contact);
        json_response_init(&resp, ERROR_MYSQL, "Failed to add contact to database");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, json);
        free(json);
        return ret;
    }

    // 添加到链表
    if (global_contact_list)
    {
        llist_insert((LLIST *)global_contact_list, contact, TAILINSERT);
    }

    char *contact_json = contact_to_json(contact);
    json_response_init(&resp, SUCCESS, "Contact added successfully");
    json_response_set_data(&resp, contact_json);
    char *json = json_response_to_string(&resp);

    int ret = send_json_response(connection, MHD_HTTP_CREATED, json);

    free(contact_json);
    free(json);
    free(contact);

    return ret;
}

// 处理PUT /api/contacts 更新联系人
static int handle_put_contact(struct MHD_Connection *connection, const char *put_data)
{
    JsonResponse resp;

    if (!put_data || strlen(put_data) == 0)
    {
        json_response_init(&resp, ERROR_INVALID_INPUT, "Empty request body");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_BAD_REQUEST, json);
        free(json);
        return ret;
    }

    Contact *contact = parse_contact_from_json(put_data);
    if (!contact || contact->id <= 0)
    {
        free(contact);
        json_response_init(&resp, ERROR_INVALID_INPUT, "Invalid contact data or missing ID");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_BAD_REQUEST, json);
        free(json);
        return ret;
    }


    // 更新数据库
    if (!db_update_contact(contact))
    {
        free(contact);
        json_response_init(&resp, ERROR_CONTACT_NOT_FOUND, "Contact not found or update failed");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_NOT_FOUND, json);
        free(json);
        return ret;
    }

    // 更新链表中的联系人
    if (global_contact_list)
    {
        Contact *existing = llist_find((LLIST *)global_contact_list, contact->id);
        if (existing)
        {
            memcpy(existing, contact, sizeof(Contact));
        }
    }

    char *contact_json = contact_to_json(contact);
    json_response_init(&resp, SUCCESS, "Contact updated successfully");
    json_response_set_data(&resp, contact_json);
    char *json = json_response_to_string(&resp);

    int ret = send_json_response(connection, MHD_HTTP_OK, json);

    free(contact_json);
    free(json);
    free(contact);

    return ret;
}

// 处理DELETE /api/contacts/{id} 删除联系人
static int handle_delete_contact(struct MHD_Connection *connection, int id)
{
    JsonResponse resp;

    if (id <= 0)
    {
        json_response_init(&resp, ERROR_INVALID_INPUT, "Invalid contact ID");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_BAD_REQUEST, json);
        free(json);
        return ret;
    }

    // 从数据库软删除
    if (!db_soft_delete_contact(id))
    {
        json_response_init(&resp, ERROR_CONTACT_NOT_FOUND, "Contact not found or delete failed");
        char *json = json_response_to_string(&resp);
        int ret = send_json_response(connection, MHD_HTTP_NOT_FOUND, json);
        free(json);
        return ret;
    }

    // 从链表中删除
    if (global_contact_list)
    {
        llist_delete((LLIST *)global_contact_list, id);
    }

    json_response_init(&resp, SUCCESS, "Contact deleted successfully");
    char *json = json_response_to_string(&resp);

    int ret = send_json_response(connection, MHD_HTTP_OK, json);

    free(json);
    return ret;
}

// 解析URL中的ID
static int parse_id_from_url(const char *url)
{
    if (!url) return -1;

    // URL格式: /api/contacts/123
    const char *prefix = "/api/contacts/";
    if (strncmp(url, prefix, strlen(prefix)) != 0)
        return -1;

    const char *id_str = url + strlen(prefix);
    return atoi(id_str);
}

// 主请求处理函数
int handle_request(void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls)
{
    // 处理CORS预检请求
    if (strcmp(method, "OPTIONS") == 0)
    {
        return handle_options(connection);
    }

    // 检查URL路径
    if (strcmp(url, "/api/contacts") == 0)
    {
        if (strcmp(method, "GET") == 0)
        {
            return handle_get_contacts(connection);
        }
        else if (strcmp(method, "POST") == 0)
        {
            // 读取POST数据
            static char post_data[4096];
            if (*con_cls == NULL)
            {
                *con_cls = post_data;
                post_data[0] = '\0';
                return MHD_YES;
            }
            else if (*upload_data_size)
            {
                strncat(post_data, upload_data, *upload_data_size);
                *upload_data_size = 0;
                return MHD_YES;
            }
            else
            {
                return handle_post_contact(connection, post_data);
            }
        }
        else if (strcmp(method, "PUT") == 0)
        {
            // 读取PUT数据
            static char put_data[4096];
            if (*con_cls == NULL)
            {
                *con_cls = put_data;
                put_data[0] = '\0';
                return MHD_YES;
            }
            else if (*upload_data_size)
            {
                strncat(put_data, upload_data, *upload_data_size);
                *upload_data_size = 0;
                return MHD_YES;
            }
            else
            {
                return handle_put_contact(connection, put_data);
            }
        }
    }
    else
    {
        // 尝试解析URL中的ID
        int id = parse_id_from_url(url);
        if (id > 0)
        {
            if (strcmp(method, "GET") == 0)
            {
                return handle_get_contact(connection, id);
            }
            else if (strcmp(method, "DELETE") == 0)
            {
                return handle_delete_contact(connection, id);
            }
        }
    }

    // 未找到的路由
    JsonResponse resp;
    json_response_init(&resp, ERROR_SYSTEM, "Route not found");
    char *json = json_response_to_string(&resp);
    int ret = send_json_response(connection, MHD_HTTP_NOT_FOUND, json);
    free(json);

    return ret;
}

// 启动HTTP服务器
int http_server_start(int port)
{
    if (server.running)
    {
        printf("HTTP server is already running\n");
        return 0;
    }

    // 创建链表
    global_contact_list = llist_create(sizeof(Contact));
    if (!global_contact_list)
    {
        printf("Failed to create contact list\n");
        return 0;
    }

    // 初始化数据库
    if (!db_init())
    {
        printf("Failed to initialize database\n");
        llist_destroy((LLIST *)global_contact_list);
        global_contact_list = NULL;
        return 0;
    }

    // 从数据库加载联系人
    if (!db_load_contacts(global_contact_list))
    {
        printf("Failed to load contacts from database\n");
    }

    // 启动HTTP服务器
    server.daemon = MHD_start_daemon(
        MHD_USE_SELECT_INTERNALLY, port, NULL, NULL,
        &handle_request, NULL,
        MHD_OPTION_END);

    if (!server.daemon)
    {
        printf("Failed to start HTTP server on port %d\n", port);
        db_close();
        llist_destroy((LLIST *)global_contact_list);
        global_contact_list = NULL;
        return 0;
    }

    server.port = port;
    server.running = 1;

    printf("HTTP server started on port %d\n", port);
    printf("Available endpoints:\n");
    printf("  GET    /api/contacts          - Get all contacts\n");
    printf("  GET    /api/contacts/{id}     - Get contact by ID\n");
    printf("  POST   /api/contacts          - Add new contact\n");
    printf("  PUT    /api/contacts          - Update contact\n");
    printf("  DELETE /api/contacts/{id}     - Delete contact by ID\n");

    return 1;
}

// 停止HTTP服务器
void http_server_stop()
{
    if (server.daemon)
    {
        MHD_stop_daemon(server.daemon);
        server.daemon = NULL;
        server.running = 0;
        printf("HTTP server stopped\n");
    }

    // 关闭数据库连接
    db_close();

    // 销毁链表
    if (global_contact_list)
    {
        llist_destroy((LLIST *)global_contact_list);
        global_contact_list = NULL;
    }
}