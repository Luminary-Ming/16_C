#include "stack.h"
#include <stdio.h>

int stack_arr[STACKSIZE];  // 使用全局数组实现栈式存储结构
int stack_ind;  // 使用全局变量作为栈顶元素的标记(默认标记为 0)

static int is_full(void)  // 栈是是否满了
{
	return !(stack_ind - STACKSIZE);
}

static int is_empty(void)  // 栈是否为空
{
	return !(stack_ind);
}

int stack_push(int data)  // 入栈
{
	if (is_full())
		return -1;  // 栈满了, 返回 -1
	stack_arr[stack_ind++] = data;  // 入栈
	return 0;
}


int stack_pop(int *save)   // 出栈
{
	if (is_empty())
		return -1;  // 栈为空, 返回 -1
	*save = stack_arr[--stack_ind];  // 出栈
	return 0;
}

void stack_display(void)  // 遍历栈
{
	int i = stack_ind;  // 使用 i 代替栈顶元素访问元素

	if (is_empty())
		return;  // 栈为空, 直接返回

	while (i--)
		printf("     %d\n", stack_arr[i]);

	printf("--------------------\n\n");
}