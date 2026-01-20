#include "joseph.h"
#include <stdio.h>
#include <stdlib.h>

JOSEPH *joseph_insert(int p_n) 
{
	JOSEPH *jo = malloc(sizeof(JOSEPH));  // 开辟第一个节点
	if (jo == NULL) return NULL;	
	jo->id = 1;  // 第一个人 id 是 1
	jo->next = jo;

	JOSEPH *p = jo;  // p 代替 jo 做操作
	for (int i = 2; i <= p_n; i++)  // 从第二个人开始开辟新节点
	{
		JOSEPH *newnode = malloc(sizeof(JOSEPH));
		newnode->id = i;
		newnode->next = p->next;
		p->next = newnode;
		p = newnode;
	}
	return jo;
}

void joseph_display(JOSEPH *jo) 
{
	JOSEPH *cur = NULL;
	for (cur = jo; cur->next != jo; cur = cur->next)
		printf("%d ", cur->id);
	printf("%d\n", cur->id);
}

int joseph_play(JOSEPH *jo, int p_n, int k_n)
{
	JOSEPH *back = NULL;   // back 指针指向要删除节点的前一个位置
	JOSEPH *cur = NULL;  // cur 指针指向要删除的节点
	
	// 找到链表的最后一个节点
	// 这样 back->next 就是第一个节点, 形成 "前一个位置" 和 "当前要删除位置" 的关系
	for (back = jo; back->next != jo; back = back->next);  

	while (--p_n)  // 循环淘汰 p_n-1 次, 剩一个人的时候停止
	{
		// 假设 k_n = 3, 数到 3 的人淘汰
		// back->next 是第一个人, 数 1, 所以数到 3 再移动两个人就行了, 也就是 k_n-1
		// 所以循环 k_n-1 次 back 就能到被淘汰的人的前一个节点
		for (int i = 1; i < k_n; i++)
			back = back->next;

		cur = back->next;  // 要删除的节点(自杀的人)
		back->next = cur->next;  // 跳过 cur 删除的节点
		printf("%d ", cur->id);  // 自杀人的 ID
		free(cur);  // 释放节点(死亡)
	}

	int alive = back->id;  // 幸存者 id
	free(back);  // 释放内存

	return alive;
}