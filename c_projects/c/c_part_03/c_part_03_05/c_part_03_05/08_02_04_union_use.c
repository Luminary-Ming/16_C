/*
	共用体的使用 :
	录入学生的成绩, 成绩有整数也有小数
*/
#include <stdio.h>

union U
{
	int i;
	float f;
};

struct Student
{
	int id;
	char name[20];
	union U socre;
};


int main(void)
{
	struct Student s;
	int flag = 0;  // 选择整数小数的标志

	printf("请输入 ID :\n");
	scanf("%d", &s.id);

	printf("请输入 name :\n");
	scanf("%s", s.name);

	printf("请输入分数(1.整数 2.小数) :\n");
	scanf("%d", &flag);
	switch (flag)
	{
		case 1: scanf("%d", &s.socre.i); break;
		case 2: scanf("%f", &s.socre.f); break;
		default:printf("输入失败, 请重试!"); break;
	}

	printf("ID : %d\n", s.id);
	printf("姓名 : %s\n", s.name);
	switch (flag)
	{
		case 1: printf("分数 : %d\n", s.socre.i); break;
		case 2: printf("分数 : %.1f\n", s.socre.f); break;
	}

	return 0;
}