#ifndef __QUEUE_H
#define __QUEUE_H

#define QUEUESIZE 6

typedef struct queue
{
	int queue_arr[QUEUESIZE];  // 队列的存储空间
	int queue_front;  // 对头的标记
	int queue_rear;  // 队尾的标记
}QUEUE;

QUEUE *queue_create(void);  // 创建
int queue_en(QUEUE *, int);  // 入队
int queue_de(QUEUE *, int *);  // 出队 
void queue_display(QUEUE *);  // 遍历
void queue_destroy(QUEUE *);  // 销毁

#endif