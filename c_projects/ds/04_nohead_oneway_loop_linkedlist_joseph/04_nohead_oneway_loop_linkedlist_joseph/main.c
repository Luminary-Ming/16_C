/*
	无头单向循环链表
	约瑟夫环
*/
#include <stdio.h>
#include <stdlib.h>
#include "joseph.h"

int main(void)
{
	int people_total = 0;  // 总人数
	int kill_num = 0;  // 数到几杀人

	printf("输入总人数: \n");
	scanf("%d", &people_total);

	printf("输入数到几杀人: \n");
	scanf("%d", &kill_num);

	JOSEPH *jo = joseph_create(people_total);  // 创建约瑟夫环
	if (!jo)
	{
		printf("Create joseph failed\n");
		return -1;
	}
		
	joseph_display(jo);  // 遍历约瑟夫环

	printf("\n====================================\n");

	int alive = joseph_play(jo, people_total, kill_num);  // 存活人的编号
	printf("\nAlive : %d\n", alive);

	return 0;
}