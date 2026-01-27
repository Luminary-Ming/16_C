#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>  // sudo apt-get install libmysqlclient-dev -y
#include <microhttpd.h>  // sudo apt-get install libmicrohttpd-dev -y
#include <jansson.h>  // sudo apt-get install libjansson-dev -y
#define PORT 8080
#define DB_HOST "10.11.17.172"
#define DB_USER "jiahao"
#define DB_PASSWD "jiahao123"
#define DB_NAME "address_book"

// 数据库连接池
MYSQL *db_conn = NULL;

// 初始化数据库连接
int init_database()
{
    db_conn = mysql_init(NULL);
    if (!db_conn)
    {
        fprintf(stderr, "mysql_init failed\n");
        return 0;
    }

    if (!mysql_real_connect(db_conn, DB_HOST, DB_USER, DB_PASSWD, DB_NAME, 0, NULL, 0))
    {
        fprintf(stderr, "mysql_connect failed: %s\n", mysql_error(db_conn));
        return 0;
    }

    mysql_set_character_set(db_conn, "utf8mb4");  // 设置字符集
    printf("数据库连接成功\n");
    return 1;
}

// 设置 CORS 头部, 处理跨域
void set_cors_headers(struct MHD_Response *response)
{
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
    MHD_add_response_header(response, "Access-Control-Allow-Methods", "*");
    MHD_add_response_header(response, "Access-Control-Allow-Headers", "*");
    MHD_add_response_header(response, "Content-Type", "application/json; charset=utf-8");
}

// 处理 OPTIONS 请求（CORS预检）
int handle_options(struct MHD_Connection *connection)
{
    struct MHD_Response *response = MHD_create_response_from_buffer(0, "", MHD_RESPMEM_PERSISTENT);
    set_cors_headers(response);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

// 发送JSON响应
int send_json_response(struct MHD_Connection *connection, json_t *json, int status_code)
{
    char *json_str = json_dumps(json, JSON_INDENT(2));
    struct MHD_Response *response = MHD_create_response_from_buffer(
        strlen(json_str), json_str, MHD_RESPMEM_MUST_FREE);
    set_cors_headers(response);
    int ret = MHD_queue_response(connection, status_code, response);
    MHD_destroy_response(response);
    return ret;
}

// 发送错误响应
int send_error(struct MHD_Connection *connection, const char *message, int status_code)
{
    json_t *error = json_object();
    json_object_set_new(error, "error", json_string(message));
    json_object_set_new(error, "success", json_false());

    int ret = send_json_response(connection, error, status_code);
    json_decref(error);
    return ret;
}

// 发送成功响应
int send_success(struct MHD_Connection *connection, json_t *data, int status_code)
{
    json_t *response = json_object();
    json_object_set_new(response, "success", json_true());
    json_object_set_new(response, "data", data);

    int ret = send_json_response(connection, response, status_code);
    json_decref(response);
    return ret;
}

// 解析请求体中的JSON
json_t *parse_request_body(const char *upload_data, size_t *upload_data_size)
{
    if (!upload_data || *upload_data_size == 0) return NULL;

    // 确保字符串以null结尾
    char *body = malloc(*upload_data_size + 1);
    memcpy(body, upload_data, *upload_data_size);
    body[*upload_data_size] = '\0';

    json_error_t error;
    json_t *json = json_loads(body, 0, &error);
    free(body);

    return json;
}

// 获取所有联系人
int get_all_contacts(struct MHD_Connection *connection)
{
    const char *query = "SELECT id, name, telephone, email, initial FROM contacts ORDER BY initial, name";

    if (mysql_query(db_conn, query))
    {
        return send_error(connection, mysql_error(db_conn), MHD_HTTP_INTERNAL_SERVER_ERROR);
    }

    MYSQL_RES *result = mysql_store_result(db_conn);
    if (!result)
    {
        return send_error(connection, "查询失败", MHD_HTTP_INTERNAL_SERVER_ERROR);
    }

    json_t *contacts = json_array();
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        json_t *contact = json_object();
        json_object_set_new(contact, "id", json_integer(atoi(row[0])));
        json_object_set_new(contact, "name", json_string(row[1]));
        json_object_set_new(contact, "telephone", json_string(row[2]));
        json_object_set_new(contact, "email", json_string(row[3]));
        json_object_set_new(contact, "initial", json_string(row[4]));
        json_array_append_new(contacts, contact);
    }

    mysql_free_result(result);
    return send_success(connection, contacts, MHD_HTTP_OK);
}

// 搜索联系人
int search_contacts(struct MHD_Connection *connection, const char *query_str)
{
    char query[512];
    char escaped_query[256];

    mysql_real_escape_string(db_conn, escaped_query, query_str, strlen(query_str));

    snprintf(query, sizeof(query),
        "SELECT id, name, telephone, email, initial FROM contacts "
        "WHERE name LIKE '%%%s%%' OR telephone LIKE '%%%s%%' "
        "ORDER BY initial, name",
        escaped_query, escaped_query);

    if (mysql_query(db_conn, query))
    {
        return send_error(connection, mysql_error(db_conn), MHD_HTTP_INTERNAL_SERVER_ERROR);
    }

    MYSQL_RES *result = mysql_store_result(db_conn);
    if (!result)
    {
        return send_error(connection, "查询失败", MHD_HTTP_INTERNAL_SERVER_ERROR);
    }

    json_t *contacts = json_array();
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        json_t *contact = json_object();
        json_object_set_new(contact, "id", json_integer(atoi(row[0])));
        json_object_set_new(contact, "name", json_string(row[1]));
        json_object_set_new(contact, "telephone", json_string(row[2]));
        json_object_set_new(contact, "email", json_string(row[3]));
        json_object_set_new(contact, "initial", json_string(row[4]));
        json_array_append_new(contacts, contact);
    }

    mysql_free_result(result);
    return send_success(connection, contacts, MHD_HTTP_OK);
}

// 创建联系人
int create_contact(struct MHD_Connection *connection, json_t *data)
{
    const char *name = json_string_value(json_object_get(data, "name"));
    const char *telephone = json_string_value(json_object_get(data, "telephone"));
    const char *email = json_string_value(json_object_get(data, "email"));
    const int del = json_string_value(json_object_get(data, "del"));

    if (!name || !telephone)
    {
        return send_error(connection, "姓名和电话不能为空", MHD_HTTP_BAD_REQUEST);
    }

    // 计算首字母
    char initial = '#';
    if (name[0] != '\0' && ((name[0] >= 'A' && name[0] <= 'Z') || (name[0] >= 'a' && name[0] <= 'z')))
    {
        initial = toupper(name[0]);
    }

    // 转义输入防止SQL注入
    char escaped_name[201], escaped_telephone[41], escaped_email[101];
    mysql_real_escape_string(db_conn, escaped_name, name, strlen(name));
    mysql_real_escape_string(db_conn, escaped_telephone, telephone, strlen(telephone));
    if (email)
    {
        mysql_real_escape_string(db_conn, escaped_email, email, strlen(email));
    }

    char query[512];
    if (email)
    {
        snprintf(query, sizeof(query),
            "INSERT INTO contacts (name, telephone, email, initial, del) "
            "VALUES ('%s', '%s', '%s', '%c', '%d')",
            escaped_name, escaped_telephone, escaped_email, initial, del);
    }
    else
    {
        snprintf(query, sizeof(query),
            "INSERT INTO contacts (name, telephone, initial, del) "
            "VALUES ('%s', '%s', '%c', '%d')",
            escaped_name, escaped_telephone, initial, del);
    }

    if (mysql_query(db_conn, query))
    {
        return send_error(connection, mysql_error(db_conn), MHD_HTTP_INTERNAL_SERVER_ERROR);
    }

    // 获取新创建的记录
    my_ulonglong new_id = mysql_insert_id(db_conn);

    json_t *response = json_object();
    json_object_set_new(response, "id", json_integer(new_id));
    json_object_set_new(response, "message", json_string("联系人创建成功"));

    return send_success(connection, response, MHD_HTTP_CREATED);
}

// 更新联系人
int update_contact(struct MHD_Connection *connection, int id, json_t *data)
{
    const char *name = json_string_value(json_object_get(data, "name"));
    const char *telephone = json_string_value(json_object_get(data, "telephone"));
    const char *email = json_string_value(json_object_get(data, "email"));
    const int *del = json_string_value(json_object_get(data, "del"));

    if (!name || !telephone)
    {
        return send_error(connection, "姓名和电话不能为空", MHD_HTTP_BAD_REQUEST);
    }

    char initial = '#';
    if (name[0] != '\0' && ((name[0] >= 'A' && name[0] <= 'Z') || (name[0] >= 'a' && name[0] <= 'z')))
    {
        initial = toupper(name[0]);
    }

    // 转义输入
    char escaped_name[201], escaped_telephone[41], escaped_email[101];
    mysql_real_escape_string(db_conn, escaped_name, name, strlen(name));
    mysql_real_escape_string(db_conn, escaped_telephone, telephone, strlen(telephone));
    if (email)
    {
        mysql_real_escape_string(db_conn, escaped_email, email, strlen(email));
    }

    char query[512];
    if (email)
    {
        snprintf(query, sizeof(query),
            "UPDATE contacts SET name='%s', telephone='%s', email='%s', initial='%c', del='%d' "
            "WHERE id=%d",
            escaped_name, escaped_telephone, escaped_email, initial, del , id);
    }
    else
    {
        snprintf(query, sizeof(query),
            "UPDATE contacts SET name='%s', telephone='%s', email=NULL, initial='%c', del='%d'  "
            "WHERE id=%d",
            escaped_name, escaped_telephone, initial, del, id);
    }

    if (mysql_query(db_conn, query))
    {
        return send_error(connection, mysql_error(db_conn), MHD_HTTP_INTERNAL_SERVER_ERROR);
    }

    if (mysql_affected_rows(db_conn) == 0)
    {
        return send_error(connection, "联系人不存在", MHD_HTTP_NOT_FOUND);
    }

    json_t *response = json_object();
    json_object_set_new(response, "message", json_string("联系人更新成功"));

    return send_success(connection, response, MHD_HTTP_OK);
}

// 删除联系人
int delete_contact(struct MHD_Connection *connection, int id)
{
    char query[100];
    snprintf(query, sizeof(query), "DELETE FROM contacts WHERE id=%d", id);

    if (mysql_query(db_conn, query))
    {
        return send_error(connection, mysql_error(db_conn), MHD_HTTP_INTERNAL_SERVER_ERROR);
    }

    if (mysql_affected_rows(db_conn) == 0)
    {
        return send_error(connection, "联系人不存在", MHD_HTTP_NOT_FOUND);
    }

    json_t *response = json_object();
    json_object_set_new(response, "message", json_string("联系人删除成功"));

    return send_success(connection, response, MHD_HTTP_OK);
}

// 主请求处理函数
int handle_request(void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls)
{

    // 静态变量存储请求体
    static char request_body[4096];
    static size_t body_size = 0;

    // 初始化连接状态
    if (*con_cls == NULL)
    {
        *con_cls = (void *)1;
        body_size = 0;
        return MHD_YES;
    }

    // 处理OPTIONS请求（CORS预检）
    if (strcmp(method, "OPTIONS") == 0)
    {
        return handle_options(connection);
    }

    // 读取请求体
    if (*upload_data_size > 0)
    {
        if (body_size + *upload_data_size < sizeof(request_body))
        {
            memcpy(request_body + body_size, upload_data, *upload_data_size);
            body_size += *upload_data_size;
        }
        *upload_data_size = 0;
        return MHD_YES;
    }

    // 解析URL和参数
    printf("请求: %s %s\n", method, url);

    // 解析请求体中的JSON（如果是POST/PUT）
    json_t *json_data = NULL;
    if ((strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0) && body_size > 0)
    {
        request_body[body_size] = '\0';
        json_data = json_loads(request_body, 0, NULL);
    }

    // 路由处理
    int response_code = MHD_HTTP_NOT_FOUND;

    if (strcmp(url, "/api/contacts") == 0)
    {
        if (strcmp(method, "GET") == 0)
        {
            response_code = get_all_contacts(connection);
        }
        else if (strcmp(method, "POST") == 0)
        {
            if (json_data)
            {
                response_code = create_contact(connection, json_data);
            }
            else
            {
                response_code = send_error(connection, "无效的请求数据", MHD_HTTP_BAD_REQUEST);
            }
        }
    }
    else if (strncmp(url, "/api/contacts/", 14) == 0)
    {
        int id = atoi(url + 14);

        if (strcmp(method, "PUT") == 0)
        {
            if (json_data)
            {
                response_code = update_contact(connection, id, json_data);
            }
            else
            {
                response_code = send_error(connection, "无效的请求数据", MHD_HTTP_BAD_REQUEST);
            }
        }
        else if (strcmp(method, "DELETE") == 0)
        {
            response_code = delete_contact(connection, id);
        }
    }
    else if (strncmp(url, "/api/contacts/search", 20) == 0)
    {
        if (strcmp(method, "GET") == 0)
        {
            const char *query = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "q");
            if (query)
            {
                response_code = search_contacts(connection, query);
            }
            else
            {
                response_code = get_all_contacts(connection);
            }
        }
    }
    else
    {
        json_t *error = json_object();
        json_object_set_new(error, "error", json_string("API端点不存在"));
        json_object_set_new(error, "success", json_false());
        response_code = send_json_response(connection, error, MHD_HTTP_NOT_FOUND);
        json_decref(error);
    }

    // 清理
    if (json_data)
    {
        json_decref(json_data);
    }

    body_size = 0;
    return response_code;
}

int main()
{
    printf("=== 联系人管理API服务器启动 ===\n");
    printf("端口: %d\n", PORT);
    printf("数据库: %s\n", DB_NAME);

    if (!init_database())
    {
        fprintf(stderr, "数据库初始化失败\n");
        return 1;
    }

    // 启动HTTP服务器
    struct MHD_Daemon *daemon = MHD_start_daemon(
        MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD,
        PORT, NULL, NULL,
        &handle_request, NULL,
        MHD_OPTION_END);

    if (!daemon)
    {
        fprintf(stderr, "启动服务器失败\n");
        mysql_close(db_conn);
        return 1;
    }

    printf("服务器已启动，监听端口 %d\n", PORT);
    printf("按任意键停止服务器...\n");

    getchar();

    MHD_stop_daemon(daemon);
    mysql_close(db_conn);
    printf("服务器已停止\n");

    return 0;
}