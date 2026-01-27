#include "db_mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "contact_llist.h"
#include "contact.h"
//#include <ctype.h>

MYSQL *db_conn = NULL;  // 数据库连接池

int db_init()
{
    // 初始化MySQL库（添加这一行）
    if (mysql_library_init(0, NULL, NULL))
    {
        fprintf(stderr, "mysql_library_init failed\n");
        return 0;
    }

    // 初始化MySQL连接
    db_conn = mysql_init(NULL);
    if (!db_conn)
    {
        fprintf(stderr, "mysql_init failed\n");
        return 0;
    }

    // 设置连接选项（可选）
    mysql_options(db_conn, MYSQL_SET_CHARSET_NAME, "utf8mb4");

    // 连接数据库
    if (!mysql_real_connect(db_conn, DB_HOST, DB_USER, DB_PASSWD, DB_NAME, 0, NULL, 0))
    {
        fprintf(stderr, "mysql_connect failed: %s\n", mysql_error(db_conn));
        mysql_close(db_conn);
        db_conn = NULL;
        return 0;
    }

    // 设置字符集
    if (mysql_set_character_set(db_conn, "utf8mb4"))
    {
        fprintf(stderr, "set charset failed: %s\n", mysql_error(db_conn));
        // 这里不直接返回，因为连接已经建立
    }

    printf("数据库连接成功\n");

    return 1;
}

int db_close()
{
    if (db_conn)
    {
        mysql_close(db_conn);
        db_conn = NULL;  // 防止使用野指针
        printf("数据库连接已关闭\n");
        mysql_library_end();  // 清理MySQL库
        return 1;
    }
    return 0;
}

/*
    查询联系人列表
*/
int db_load_contacts(LLIST *handler)
{
    if (!db_conn || !handler)
        return 0;
    const char *query = "select id,name,telephone,email,initial,del from contacts where del = 0";

    if (mysql_query(db_conn, query))
    {
        fprintf(stderr, "查询失败: %s\n", mysql_error(db_conn));
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(db_conn);
    if (!result)
    {
        fprintf(stderr, "获取结果失败: %s\n", mysql_error(db_conn));
        return 0;
    }

    MYSQL_ROW row;
    int count = 0;

    // 清空链表
    struct llist_node *cur = handler->head.next;
    while (cur != &handler->head)
    {
        struct llist_node *next = cur->next;  // 修复：先保存下一个节点
        free(cur);  // 释放数据节点
        cur = next;
    }
    handler->head.next = &handler->head;
    handler->head.prev = &handler->head;
    handler->count = 0;

    while ((row = mysql_fetch_row(result)))
    {
        Contact contact;

        contact.id = atoi(row[0]);

        strncpy(contact.name, row[1] ? row[1] : "", NAMESIZE - 1);
        contact.name[NAMESIZE - 1] = '\0';

        strncpy(contact.telephone, row[2] ? row[2] : "", TELEPHONESIZE - 1);
        contact.telephone[TELEPHONESIZE - 1] = '\0';  // 修复：应该是telephone

        strncpy(contact.email, row[3] ? row[3] : "", EMAILSIZE - 1);
        contact.email[EMAILSIZE - 1] = '\0';  // 修复：应该是email

        contact.initial = row[4] ? row[4][0] : '#';

        contact.del = row[5] ? atoi(row[5]) : 0;

        llist_insert(handler, &contact, TAILINSERT);
        count++;
    }
    mysql_free_result(result);
    printf("从数据库加载了 %d 个联系人\n", count);
    return 1;
}

// 添加联系人到数据库
int db_add_contact(Contact *contact)
{
    if (!db_conn || !contact)
        return 0;

    char query[1024];
    contact->initial = get_initial(contact->name);

    snprintf(query, sizeof(query),
        "insert into contacts (name, telephone, email, initial, del) "
        "values ('%s', '%s', '%s', '%c', '%d')",
        contact->name, contact->telephone, contact->email, contact->initial, contact->del);

    if (mysql_query(db_conn, query))
    {
        fprintf(stderr, "插入失败: %s\n", mysql_error(db_conn));
        return 0;
    }

    contact->id = (int)mysql_insert_id(db_conn);
    contact->del = 0;

    printf("添加联系人成功，ID: %d\n", contact->id);
    return 1;
}

// 更新联系人
int db_update_contact(Contact *contact)
{
    if (!db_conn || !contact || contact->id <= 0)
        return 0;

    char query[1024];
    contact->initial = get_initial(contact->name);

    snprintf(query, sizeof(query),
        "UPDATE contacts SET name='%s', telephone='%s', email='%s', initial='%c' "
        "WHERE id=%d AND del=0",
        contact->name, contact->telephone, contact->email, contact->initial, contact->id);

    if (mysql_query(db_conn, query))
    {
        fprintf(stderr, "更新失败: %s\n", mysql_error(db_conn));
        return 0;
    }

    if (mysql_affected_rows(db_conn) > 0)
    {
        printf("更新联系人成功，ID: %d\n", contact->id);
        return 1;
    }
    else
    {
        printf("未找到联系人，ID: %d\n", contact->id);
        return 0;
    }
}

// 软删除联系人（标记删除）
int db_soft_delete_contact(int id)
{
    if (!db_conn || id <= 0)
        return 0;

    char query[256];
    snprintf(query, sizeof(query), "UPDATE contacts SET del=1 WHERE id=%d", id);

    if (mysql_query(db_conn, query))
    {
        fprintf(stderr, "删除失败: %s\n", mysql_error(db_conn));
        return 0;
    }

    if (mysql_affected_rows(db_conn) > 0)
    {
        printf("删除联系人成功，ID: %d\n", id);
        return 1;
    }
    else
    {
        printf("未找到联系人，ID: %d\n", id);
        return 0;
    }
}

// 硬删除联系人（从数据库彻底删除）
int db_delete_contact(int id)
{
    if (!db_conn || id <= 0)
        return 0;

    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM contacts WHERE id=%d", id);

    if (mysql_query(db_conn, query))
    {
        fprintf(stderr, "删除失败: %s\n", mysql_error(db_conn));
        return 0;
    }

    if (mysql_affected_rows(db_conn) > 0)
    {
        printf("彻底删除联系人成功，ID: %d\n", id);
        return 1;
    }
    else
    {
        printf("未找到联系人，ID: %d\n", id);
        return 0;
    }
}

// 查找联系人
Contact *db_find_contact(int id)
{
    if (!db_conn || id <= 0)
        return NULL;

    char query[256];
    snprintf(query, sizeof(query),
        "SELECT id, name, telephone, email, initial, del FROM contacts WHERE id=%d AND del=0", id);

    if (mysql_query(db_conn, query))
    {
        fprintf(stderr, "查询失败: %s\n", mysql_error(db_conn));
        return NULL;
    }

    MYSQL_RES *result = mysql_store_result(db_conn);
    if (!result)
        return NULL;

    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row)
    {
        mysql_free_result(result);
        return NULL;
    }

    Contact *contact = malloc(sizeof(Contact));
    if (!contact)
    {
        mysql_free_result(result);
        return NULL;
    }

    contact->id = atoi(row[0]);

    if (row[1])
    {
        strncpy(contact->name, row[1], NAMESIZE - 1);
        contact->name[NAMESIZE - 1] = '\0';
    }
    else
    {
        contact->name[0] = '\0';
    }

    if (row[2])
    {
        strncpy(contact->telephone, row[2], TELEPHONESIZE - 1);
        contact->telephone[TELEPHONESIZE - 1] = '\0';
    }
    else
    {
        contact->telephone[0] = '\0';
    }

    if (row[3])
    {
        strncpy(contact->email, row[3], EMAILSIZE - 1);
        contact->email[EMAILSIZE - 1] = '\0';
    }
    else
    {
        contact->email[0] = '\0';
    }

    contact->initial = row[4] ? row[4][0] : '#';
    contact->del = row[5] ? atoi(row[5]) : 0;

    mysql_free_result(result);
    return contact;
}