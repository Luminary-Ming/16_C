#include "stack.h"
#include <stdlib.h>

STACK *stack_create(void)
{
	STACK *s = NULL;
	s = malloc(sizeof(STACK));  // 开辟栈式存储结构
	if (s == NULL)
		return NULL;
	s->stack_ind = 0;  // 初始化栈顶元素
	return s;
}

int is_full(STACK * s)
{
	return !(s->stack_ind - STACKSIZE);
}

int is_empty(STACK *s)
{
	return !(s->stack_ind);
}

int stack_push(STACK *s, int data)
{
	if (is_full(s))
		return -1;
	s->stack_arr[s->stack_ind++] = data;  // 入栈
	return 0;
}

int stack_pop(STACK *s, int *save)
{
	if (is_empty(s))
		return -1;
	*save = s->stack_arr[--s->stack_ind];
	return 0;
}

void stack_destroy(STACK *s)
{
	if (s == NULL)
		return;
	free(s);  // 释放栈
}