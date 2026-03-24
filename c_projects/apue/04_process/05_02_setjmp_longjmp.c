#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

jmp_buf env;  // 存储有关调用环境的各种信息
int *p = NULL;
FILE *fp = NULL;
int fd = 0;

void make_money(void);

int main(void)
{
	switch(setjmp(env))
	{ 
		case 0:
			printf("我养你呀 !!!!");
			make_money();  // 搞钱
			printf("美滋滋 !\n");
			break;
		case 1:
			puts("算了吧 !");
			fclose(fp);
			fp = NULL;
		case 2:
			puts("还是洗洗睡吧 !");
			free(p);
			p == NULL;
		case 3:
			puts("先养好你自己吧 !");
			break;
	}

	return 0;
}

void make_money(void)
{
	p = malloc(4);  // 买房子
	if (p == NULL)
		longjmp(env, 3);

	fp = fopen("./car", "r");  // 买车子
	if(fp == NULL)
		longjmp(env, 2);

	fd = open("./jewelry", O_RDONLY);  // 买珠宝
	if(fd < 0)
		longjmp(env, 1);
}

