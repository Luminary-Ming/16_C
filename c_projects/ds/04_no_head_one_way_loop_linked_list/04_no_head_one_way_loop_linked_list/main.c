/*
	无头单向循环列表
	约瑟夫环
*/
#include <stdio.h>
#include "joseph.h"

int main(void)
{
	JOSEPH *jo = NULL;  // jo 指向约瑟夫环
	int people_num = 0;  // 总人数
	int kill_num = 0;  // 数到几杀人
	int alive = 0;  // 存活人的 id

	printf("输入总人数: \n");
	scanf("%d", &people_num);

	jo = joseph_insert(people_num);  // 创建约瑟夫环
	if (jo == NULL) return -1;

	joseph_display(jo);  // 遍历约瑟夫环

	printf("输入数到几杀人: \n");
	scanf("%d", &kill_num);

	alive = joseph_play(jo, people_num, kill_num);
	printf("\n======================\n");
	printf("Alive : %d\n", alive);

	return 0;
}