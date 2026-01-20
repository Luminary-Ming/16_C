#ifndef __JOSEPH_H
#define __JOSEPH_H

typedef struct llist_node
{
	int id;  // 人员编号
	struct llist_node *next;
}JOSEPH;  // struct llist_node 结构体类型的别名

JOSEPH *joseph_insert(int p_n);

void joseph_display(JOSEPH *jo);

int joseph_play(JOSEPH *jo, int p_n, int k_n);

#endif