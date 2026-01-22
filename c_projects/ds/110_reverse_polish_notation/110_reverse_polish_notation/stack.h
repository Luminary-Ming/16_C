#ifndef __STACK_H
#define __STACK_H

#define STACKSIZE 32

typedef struct stack
{
	int stack_arr[STACKSIZE];  // 栈式存储结构
	int stack_ind;  // 栈顶元素
}STACK;

STACK *stack_create(void);

int is_full(STACK *);

int is_empty(STACK *);

int stack_push(STACK *, int);

int stack_pop(STACK *, int *);

void stack_destroy(STACK *);

#endif