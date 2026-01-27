#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "address_book.h"

static void print_contact(const Contact *c)
{
    if (!c)
        return;
    printf("%d\t%s\t%s\t%s\n", c->id, c->name, c->phone, c->email);
}

int main(int argc, char **argv)
{
    /* 示例：请根据实际 DB 配置修改 */
    if (ab_init("127.0.0.1", "root", "", "address_book", 0) != AB_OK)
    {
        fprintf(stderr, "无法连接数据库\n");
        return 1;
    }

    printf("添加测试联系人...\n");
    Contact c = {0};
    strncpy(c.name, "张三", sizeof(c.name) - 1);
    strncpy(c.phone, "13800000000", sizeof(c.phone) - 1);
    strncpy(c.email, "zhangsan@example.com", sizeof(c.email) - 1);
    int new_id = 0;
    if (ab_add_contact(&c, &new_id) == AB_OK)
        printf("添加成功 id=%d\n", new_id);

    printf("所有联系人:\n");
    Contact *list = ab_search(NULL);
    for (Contact *it = list; it; it = it->next)
        print_contact(it);
    ab_free_list(list);

    printf("模糊查询 '张':\n");
    list = ab_search("张");
    for (Contact *it = list; it; it = it->next)
        print_contact(it);
    ab_free_list(list);

    printf("更新联系人示例...\n");
    if (new_id)
    {
        Contact u = {.id = new_id};
        strncpy(u.name, "张三丰", sizeof(u.name) - 1);
        strncpy(u.phone, "13911112222", sizeof(u.phone) - 1);
        strncpy(u.email, "zs@example.com", sizeof(u.email) - 1);
        ab_update_contact(&u);
    }

    printf("删除示例...\n");
    if (new_id)
        ab_delete_contact(new_id);

    ab_close();
    return 0;
}
