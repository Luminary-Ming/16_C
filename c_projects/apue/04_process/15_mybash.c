#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define LNAME_SIZE 128
#define HNAME_SIZE 128
#define PWD_SIZE 256
#define BUF_SIZE 16

static int get_env_pwd(char *lname, char *hname, char *pwd);  // 获取终端信息
static int get_cmd_line(char **line, size_t *length);  // 读取一行指令
static int parse_string(char *str, char *delim, char **buf, int size);  // 分割字符串
static int change_directory(char **args);  // 执行 cd 命令

int main(void)
{
	char lname[LNAME_SIZE] = {0};  // 存储当前登录用户的用户名
	char hname[HNAME_SIZE] = {0};  // 存储当前终端的主机名
	char pwd[PWD_SIZE] = {0};  // 存储当前工作目录的路径
	char *line = NULL;  // 存储读取一行指令的首地址
	size_t length = 0;  // 存储读取的字节数
	char *buf[BUF_SIZE] = {0};  // 存储

	while (1)  // 死循环
	{
		// [1] 打印终端提示符 "用户名@主机名:当前工作路径"
		if (get_env_pwd(lname, hname, pwd) < 0)  // 获取终端信息
			exit(1);
		else
			printf("%s@%s:%s$ ", lname, hname, pwd);  // 打印终端提示符

		fflush(stdout);  // 刷新 stdout 缓冲区, 确保提示符立即显示

		// [2] 等待读一行输入,例如 : "ls -l\n"
		if(get_cmd_line(&line, &length) < 0)  // 读取一行指令
			exit(2);

		line[strcspn(line, "\n")] = '\0';  // 去除末尾的换行符

		// 如果是空命令 (只按回车), 继续循环
		if (strlen(line) == 0)
		{
			free(line);
			line = NULL;
			length = 0;
			continue;
		}
		
		// 如果是 "exit" 就直接跳出循环
		if (strcmp(line, "exit") == 0)
			break;

		// [3] 分割字符串, 把 "ls -l", 分割出 "ls" "-l"
		// 使用 " \t" 作为分隔符, 支持空格和制表符
		parse_string(line, " \t", buf, BUF_SIZE);  // 分割命令 选项 参数

		// 检查是否成功分割出命令
		if (buf[0] == NULL)
		{
			free(line);
			line = NULL;
			length = 0;
			continue;
		}

		// [4] 处理内部 cd 命令
		if (strcmp(buf[0], "cd") == 0)
		{
			change_directory(buf);
			
			// cd 命令执行后继续循环, 不需要创建子进程
			free(line);
			line = NULL;
			length = 0;
			continue;
		}

		// [5] 创建子进程
		pid_t pid = fork();  // 创建子进程
		if (pid < 0)
		{
			perror("fork()");  // 打印错误信息
			exit(3);
		}

		// [6] 替换子进程
		if (pid == 0)
		{
			execvp(buf[0], buf);  // 替换子进程
			perror("execvp()");  // 替换失败, 打印错误信息
			exit(4);  // 替换失败, 终止进程, 返回状态 4
		}
		// [7] 父进程等待子进程结束
		else
			wait(NULL);

		free(line);  // 释放空间
		line = NULL;
		length = 0;
	}

	return 0;
}

// 获取终端信息
static int get_env_pwd(char *lname, char *hname, char *pwd)
{
	char *logname = getenv("LOGNAME");  // 获取当前登陆用户的用户名
	if (logname == NULL)
		return -1;
	strcpy(lname, logname);  // 拷贝给 lname

	if (gethostname(hname, HNAME_SIZE) < 0)  // 获取当前终端的主机名
	{
		perror("gethostname()");
		return -2;
	}

	if (getcwd(pwd, PWD_SIZE) == NULL)  // 获取当前工作目录的路径
	{
		perror("getcwd()");
		return -3;
	}

	return 0;
}

// 读取一行指令
static int get_cmd_line(char **line, size_t *length)
{
	if (getline(line, length, stdin) == -1)  // 获取整行命令
	{
		perror("getline()");
		return -1;
	}

	return 0;
}

// 分割字符串
static int parse_string(char *str, char *delim, char **buf, int size)
{
	int i = 0;
	while (i < size)
	{
		buf[i] = strtok(str, delim);  // 分割字符串
		if (buf[i] == NULL)  // 检查分割是否完成
			break;
		i++;
		str = NULL;  // 首次调用指向原串, 之后调用需要指向 NULL
	}
	
	return 0;
}

// 执行 cd 命令
static int change_directory(char **args)
{

	char *path = NULL;

	// [1] 只有 cd 没有参数或者 "cd ~" -> 切换到家目录
	if (args[1] == NULL || (strcmp(args[1], "~") == 0))
	{
		char *home = getenv("HOME");
		if (home == NULL)
		{
			fprintf(stderr, "cd: 你家没了 !\n");
			return -1;
		}
		path = home;
	}
	// [2] "cd -" -> 切换到上一个工作目录
	else if (strcmp(args[1], "-") == 0)
	{
		char *oldpwd = getenv("OLDPWD");
		if (oldpwd == NULL)
		{
			fprintf(stderr, "cd: 没有 oldpwd !\n");
			return -2;
		}
		path = oldpwd;
		printf("%s\n", oldpwd);  // 打印显示 oldpwd 目录
	}
	// [3] 其他情况 (包括 "cd .", "cd ..", "cd /", "cd 目录名" 等)
	else
		path = args[1];

	// 获取当前目录
	char old_cwd[PWD_SIZE];
	if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
	{
		perror("getcwd()");
		return -3;
	}

	// 切换目录
	if (chdir(path) != 0)
	{
		perror("cd");
		return -4;
	}

	// 设置 OLDPWD 环境变量 (用于 cd -)
	if (setenv("OLDPWD", old_cwd, 1) != 0)
	{
		perror("setenv()");
		return -5;
	}

	// 更新 pwd 环境变量
	char new_cwd[PWD_SIZE];
	if (getcwd(new_cwd, sizeof(new_cwd)) != NULL)
		setenv("PWD", new_cwd, 1);
	
	return 0;
}

