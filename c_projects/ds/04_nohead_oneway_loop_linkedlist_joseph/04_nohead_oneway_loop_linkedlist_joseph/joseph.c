#include "joseph.h"
#include <stdlib.h>
#include <stdio.h>

JOSEPH *joseph_create(int people_total)
{
	JOSEPH *jo = malloc(sizeof(JOSEPH));
	if (!jo)
		return NULL;
	jo->id = 1;
	jo->next = jo;  // 无头单向循环链表

	JOSEPH *p = jo;  // p 代替 jo 做操作
	for (int i = 2; i <= people_total; i++)
	{
		JOSEPH *newnode = malloc(sizeof(JOSEPH));
		newnode->id = i;
		newnode->next = p->next;
		p->next = newnode;
		p = p->next;
	}
	return jo;
}

void joseph_display(JOSEPH *jo)
{
	JOSEPH *cur = jo;  // cur 指针指向每一个数据节点
	while (cur->next != jo)
	{
		printf("%d ", cur->id);
		cur = cur->next;
	}
	printf("%d ", cur->id);  // 打印最后一个人的编号
}

int joseph_play(JOSEPH *jo, int people_total, int kill_num)
{
	JOSEPH *cur = jo;  // cur 指针指向要删除的节点
	JOSEPH *back = jo;   // back 指针指向要删除节点的前一个位置

	// 初始 cur 指向第一个节点, 所有 back 要指向最后一个节点
	// 找到链表的最后一个节点 
	while (back->next != jo)
		back = back->next;

	while (--people_total)  // 循环淘汰 people_total-1 次, 剩一个人时停止
	{
		// 假设 kill_num = 3, 数到 3 的人淘汰
		// back->next 是第一个人, 数 1, 所以数到 3 再数俩人就行了
		// 所以循环 kill_num-1 次 back 就能到被淘汰的人的前一个节点
		for (int i = 1; i < kill_num; i++)
			back = back->next;

		cur = back->next;  // 要删除的节点(淘汰的人)
		back->next = cur->next;  // 摘除要删除的节点
		printf("%d ", cur->id);  // 打印淘汰人的 ID
		free(cur);  // 删除节点
	}

	int alive = back->id;  // 幸存者 id
	free(back);  // 释放内存
	return alive;
}