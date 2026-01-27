#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define NAME_MAX_LEN 64
#define PHONE_MAX_LEN 32
#define EMAIL_MAX_LEN 128

    typedef struct Contact
    {
        int id;
        char name[NAME_MAX_LEN];
        char phone[PHONE_MAX_LEN];
        char email[EMAIL_MAX_LEN];
        struct Contact *next;
    } Contact;

    typedef enum
    {
        AB_OK = 0,
        AB_DB_CONN_FAIL = 1,
        AB_DB_QUERY_FAIL = 2,
        AB_NOT_FOUND = 3,
        AB_INVALID_ARG = 4,
        AB_ALLOC_FAIL = 5
    } AB_ERR;

    /* 初始化, host,user,pass,db,port(0表示默认3306) */
    AB_ERR ab_init(const char *host, const char *user, const char *pass, const char *db, unsigned int port);
    void ab_close(void);

    /* CRUD */
    AB_ERR ab_add_contact(const Contact *c, int *out_id);
    AB_ERR ab_update_contact(const Contact *c);
    AB_ERR ab_delete_contact(int id);

    /* 模糊查询：通过姓名或手机号模糊查询，term 可以为 NULL 表示返回全部。返回链表头（需要调用 ab_free_list 释放） */
    Contact *ab_search(const char *term);
    void ab_free_list(Contact *head);

#ifdef __cplusplus
}
#endif

#endif /* ADDRESS_BOOK_H */
