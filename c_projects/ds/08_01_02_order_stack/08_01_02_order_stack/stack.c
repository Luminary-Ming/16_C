#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

STACK *stack_create(void) 
{
	STACK *s = NULL;  // s 指针指向开辟的栈式存储结构
	s = malloc(sizeof(STACK));  // 开辟栈式存储结构
	if (s == NULL)
		return NULL;  // 开辟失败, 返回 NULL

	s->stack_ind = 0;  // 初始化栈顶元素

	return s;  // 返回开辟的栈式存储结构
}

static int is_full(STACK *s)
{
	return !(s->stack_ind - STACKSIZE);
}

int stack_push(STACK * s, int data)
{
	if (is_full(s))
		return -1;  // 栈满了, 返回 -1
	s->stack_arr[s->stack_ind++] = data;  // 入栈
	return 0;
}

static int is_empty(STACK *s)
{
	return !(s->stack_ind);
}

int stack_pop(STACK *s, int *save)
{
	if (is_empty(s))
		return -1;
	*save = s->stack_arr[--s->stack_ind];
	return 0;
}

void stack_display(STACK *s)
{
	int i = s->stack_ind;  // i 代替栈顶元素访问栈式存储空间
	
	if (is_empty(s))
		return;

	while (i--)
		printf("    %d\n", s->stack_arr[i]);

	printf("--------------------\n\n");
}

void stack_destroy(STACK *s)
{
	if (s == NULL)
		return;
	free(s);  // 释放栈
}