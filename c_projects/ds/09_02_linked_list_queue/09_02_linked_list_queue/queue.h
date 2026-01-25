#ifndef __QUEUE_H
#define __QUEUE_H

#include "llist.h"

typedef LLIST QUEUE;

QUEUE *queue_create(void);  // 创建
int queue_en(QUEUE *, int);  // 入队
int queue_de(QUEUE *, int *);  // 出队 
void queue_display(QUEUE *);  // 遍历
void queue_destroy(QUEUE *);  // 销毁

#endif