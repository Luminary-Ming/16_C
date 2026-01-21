#ifndef __STACK_H
#define __STACK_H

#define STACKSIZE 6

typedef struct stack
{
	int stack_arr[STACKSIZE];  // 栈式的存储空间
	int stack_ind;  // 栈顶元素
}STACK;

STACK *stack_create(void);

int stack_push(STACK *, int);

int stack_pop(STACK *, int *);

void stack_display(STACK *);

void stack_destroy(STACK *);

#endif