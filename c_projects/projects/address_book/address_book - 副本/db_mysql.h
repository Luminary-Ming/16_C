#ifndef __DB_MYSQL_H
#define __DB_MYSQL_H
#include "contact.h"
#include "contact_llist.h"

#define DB_HOST "10.11.17.172"  // 主机地址
#define DB_USER "jiahao"        // 用户名
#define DB_PASSWD "jiahao123"   // 密码
#define DB_NAME "address_book"  // 数据库名

int db_init();   // 初始化连接数据库
int db_close();  // 关闭数据库连接

int db_load_contacts(LLIST *handler);  // 查询联系人
int db_add_contact(Contact *contact);  // 添加联系人到数据表
int db_update_contact(Contact *contact);  // 更新联系人到数据表
int db_delete_contact(int id);  // 根据 id 删除联系人
int db_soft_delete_contact(int id);  // 软删除
Contact *db_find_contact(int id);

#endif