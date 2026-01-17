/*
	结构体类型的指针 结构体类型的数组
*/
#include <stdio.h>
#include <string.h>

typedef struct LOL
{
	char name[20];
	int level;
	int money;
}Hero;

void print(Hero *p)
{
	for (int i = 0; i < 3; i++)
	{
		printf("英雄 : %s\n", p[i].name);
		printf("等级 : %d\n", p[i].level);
		printf("金币 : %d\n\n", p[i].money);
	}
}


void change(Hero *p) // 结构体类型的指针
{
	for (int i = 0; i < 3; i++)
	{
		scanf("%s", (p + i)->name);
		scanf("%d", &(p + i)->level);
		scanf("%d", &(p + i)->money);
	}
}

int main(void)
{
	// 结构体类型的数组
	Hero arr[3] = { {"yasuo", 18, 6300},{"jiansheng", 17, 450},{"aixi", 16, 450} };

	print(arr);

	change(arr);

	print(arr);

	return 0;
}