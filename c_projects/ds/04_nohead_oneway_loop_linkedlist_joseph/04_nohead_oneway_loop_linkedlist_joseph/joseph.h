#ifndef JOSEPH_H
#define JOSEPH_H

typedef struct llist_node
{
	int id;  // 人员编号
	struct llist_node *next;
}JOSEPH;  // struct llist_node 结构体类型的别名

JOSEPH *joseph_create(int people_total);  // 创建约瑟夫环

void joseph_display(JOSEPH *jo);  // 遍历约瑟夫环

int joseph_play(JOSEPH *jo, int people_total, int kill_num);  // 进行约瑟夫环杀人游戏

#endif