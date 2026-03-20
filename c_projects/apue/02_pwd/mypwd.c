#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>  // getpass()
#include <shadow.h>  // getspnam()
#include <crypt.h>  // crypt()

#define NAMESIZE 32  // 用户名最大长度

int main(void)
{
	char name[NAMESIZE] = { 0 };  // 存储用户名

	// 输入用户名
	printf("请输入用户名："); 
	fgets(name, NAMESIZE, stdin);
	name[strcspn(name, "\n")] = '\0';  // 去除 fgets() 读取时自动添加的换行符
	//*strchr(name, '\n') = '\0';  // 把输入结尾的 '\n' 替换成 '\0' 

	// 输入原文密码
	char *pwd = getpass("请输入密码：");  // 隐藏式密码输入
	if (pwd == NULL)
	{
		perror("getpass()");
		return -1;
	}

	// 读取密文密码
	struct spwd *tp = getspnam(name);  // 从 /etc/shadow 获取该用户的加密密码
	if (tp == NULL)
	{
		fprintf(stderr, "获取 shadow 文件中的用户信息失败\n");
		return -2;
	}

	// 原文密码加密
	char *cp = crypt(pwd, tp->sp_pwdp);  // 使用 shadow 中的 salt 进行加密
	if (cp == NULL)
	{
		perror("crypt()");
		return -3;
	}

	// 对比加密结果与 shadow 中的密码
	if(!strcmp(tp->sp_pwdp, cp))
		printf("登录成功 ！\n");
	else
		printf("密码错误 ！\n");

	return 0;
}
/*
	需要root权限运行

	xinming@02_pwd$ sudo ./mypwd 
	请输入用户名：xinming
	请输入密码：
	登录成功 ！
	xinming@02_pwd$ 
*/
