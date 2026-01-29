#ifndef __CONTACT_H
#define __CONTACT_H

#define NAMESIZE 20
#define TELEPHONESIZE 12
#define EMAILSIZE 50
#define IMAGESIZE 256

typedef struct contact_node  // 联系人结构体
{
	int id;  // ID
	char name[NAMESIZE];  // 姓名
	char telephone[TELEPHONESIZE];  // 手机号
	char email[EMAILSIZE];  // 邮箱
	char initial;  // 名字首字母
	char image[IMAGESIZE];  // 头像 URL
	int del;  // 删除标识 1已删除 0未删除
}Contact;

char get_initial(const char *name);  // 获取名字首字母
void contact_print(Contact *contact);  // 打印联系人

#endif