#include "address_book.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* MySQL C API header: on many systems it is <mysql/mysql.h> or <mysql.h> */
#ifdef __has_include
#if __has_include(<mysql/mysql.h>)
#include <mysql/mysql.h>
#elif __has_include(<mysql.h>)
#include <mysql.h>
#else
#error "MySQL client headers not found, install libmysqlclient-dev or MySQL Connector/C"
#endif
#else
#include <mysql/mysql.h>
#endif

static MYSQL *g_conn = NULL;

AB_ERR ab_init(const char *host, const char *user, const char *pass, const char *db, unsigned int port)
{
    if (g_conn)
        return AB_OK;
    g_conn = mysql_init(NULL);
    if (!g_conn)
        return AB_DB_CONN_FAIL;
    if (!mysql_real_connect(g_conn, host, user, pass, db, port, NULL, 0))
    {
        fprintf(stderr, "mysql_real_connect error: %s\n", mysql_error(g_conn));
        mysql_close(g_conn);
        g_conn = NULL;
        return AB_DB_CONN_FAIL;
    }
    return AB_OK;
}

void ab_close(void)
{
    if (g_conn)
    {
        mysql_close(g_conn);
        g_conn = NULL;
    }
}

static char *escape_and_quote(const char *s)
{
    if (!s)
    {
        char *p = malloc(3);
        if (!p)
            return NULL;
        strcpy(p, "''");
        return p;
    }
    unsigned long len = (unsigned long)strlen(s);
    char *buf = malloc(len * 2 + 3);
    if (!buf)
        return NULL;
    unsigned long out_len = mysql_real_escape_string(g_conn, buf + 1, s, len);
    buf[0] = '\'';
    buf[out_len + 1] = '\'';
    buf[out_len + 2] = '\0';
    return buf;
}

AB_ERR ab_add_contact(const Contact *c, int *out_id)
{
    if (!g_conn || !c)
        return AB_INVALID_ARG;
    char *name = escape_and_quote(c->name);
    char *phone = escape_and_quote(c->phone);
    char *email = escape_and_quote(c->email);
    if (!name || !phone || !email)
    {
        free(name);
        free(phone);
        free(email);
        return AB_ALLOC_FAIL;
    }

    char q[1024];
    snprintf(q, sizeof(q), "INSERT INTO contacts (name, phone, email) VALUES (%s, %s, %s)", name, phone, email);
    free(name);
    free(phone);
    free(email);

    if (mysql_query(g_conn, q))
    {
        fprintf(stderr, "insert error: %s\n", mysql_error(g_conn));
        return AB_DB_QUERY_FAIL;
    }
    if (out_id)
        *out_id = (int)mysql_insert_id(g_conn);
    return AB_OK;
}

AB_ERR ab_update_contact(const Contact *c)
{
    if (!g_conn || !c)
        return AB_INVALID_ARG;
    if (c->id <= 0)
        return AB_INVALID_ARG;
    char *name = escape_and_quote(c->name);
    char *phone = escape_and_quote(c->phone);
    char *email = escape_and_quote(c->email);
    if (!name || !phone || !email)
    {
        free(name);
        free(phone);
        free(email);
        return AB_ALLOC_FAIL;
    }

    char q[1024];
    snprintf(q, sizeof(q), "UPDATE contacts SET name=%s, phone=%s, email=%s WHERE id=%d", name, phone, email, c->id);
    free(name);
    free(phone);
    free(email);

    if (mysql_query(g_conn, q))
    {
        fprintf(stderr, "update error: %s\n", mysql_error(g_conn));
        return AB_DB_QUERY_FAIL;
    }
    if (mysql_affected_rows(g_conn) == 0)
        return AB_NOT_FOUND;
    return AB_OK;
}

AB_ERR ab_delete_contact(int id)
{
    if (!g_conn)
        return AB_INVALID_ARG;
    if (id <= 0)
        return AB_INVALID_ARG;
    char q[256];
    snprintf(q, sizeof(q), "DELETE FROM contacts WHERE id=%d", id);
    if (mysql_query(g_conn, q))
    {
        fprintf(stderr, "delete error: %s\n", mysql_error(g_conn));
        return AB_DB_QUERY_FAIL;
    }
    if (mysql_affected_rows(g_conn) == 0)
        return AB_NOT_FOUND;
    return AB_OK;
}

Contact *ab_search(const char *term)
{
    if (!g_conn)
        return NULL;
    char q[1024];
    if (!term || strlen(term) == 0)
    {
        snprintf(q, sizeof(q), "SELECT id, name, phone, email FROM contacts ORDER BY id DESC");
    }
    else
    {
        /* 使用 LIKE 进行模糊查询，匹配 name 或 phone */
        char esc[512];
        mysql_real_escape_string(g_conn, esc, term, (unsigned long)strlen(term));
        char pattern[520];
        snprintf(pattern, sizeof(pattern), "%%%s%%", esc);
        /* 为查询形成 SQL 字面量 */
        char *patt_quoted = malloc(strlen(pattern) * 2 + 3);
        if (!patt_quoted)
            return NULL;
        unsigned long out_len = mysql_real_escape_string(g_conn, patt_quoted + 1, pattern, (unsigned long)strlen(pattern));
        patt_quoted[0] = '\'';
        patt_quoted[out_len + 1] = '\'';
        patt_quoted[out_len + 2] = '\0';
        snprintf(q, sizeof(q), "SELECT id, name, phone, email FROM contacts WHERE name LIKE %s OR phone LIKE %s ORDER BY id DESC", patt_quoted, patt_quoted);
        free(patt_quoted);
    }

    if (mysql_query(g_conn, q))
    {
        fprintf(stderr, "select error: %s\n", mysql_error(g_conn));
        return NULL;
    }
    MYSQL_RES *res = mysql_store_result(g_conn);
    if (!res)
        return NULL;

    MYSQL_ROW row;
    Contact *head = NULL;
    Contact *tail = NULL;
    while ((row = mysql_fetch_row(res)))
    {
        Contact *n = calloc(1, sizeof(Contact));
        if (!n)
        {
            ab_free_list(head);
            mysql_free_result(res);
            return NULL;
        }
        n->id = row[0] ? atoi(row[0]) : 0;
        if (row[1])
            strncpy(n->name, row[1], NAME_MAX_LEN - 1);
        if (row[2])
            strncpy(n->phone, row[2], PHONE_MAX_LEN - 1);
        if (row[3])
            strncpy(n->email, row[3], EMAIL_MAX_LEN - 1);
        n->next = NULL;
        if (!head)
            head = tail = n;
        else
        {
            tail->next = n;
            tail = n;
        }
    }
    mysql_free_result(res);
    return head;
}

void ab_free_list(Contact *head)
{
    Contact *cur = head;
    while (cur)
    {
        Contact *n = cur->next;
        free(cur);
        cur = n;
    }
}
