#ifndef QUEUE_H
#define QUEUE_H

#include "llist.h"

typedef LLIST QUEUE;

QUEUE *queue_create(void);
int queue_en(QUEUE *queue, int data);
int queue_de(QUEUE *queue, int *save);
void queue_display(QUEUE *queue);
void queue_destroy(QUEUE *queue);

#endif