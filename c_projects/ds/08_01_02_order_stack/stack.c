#include"stack.h"
#include <stdio.h>
#include <stdlib.h>

// 栈是是否满了
static int is_full(STACK *stack)
{
	return stack->stack_ind == STACKSIZE;
}

// 栈是否是空栈
static int is_empty(STACK *stack)
{
	return stack->stack_ind == 0;
}

// 创建
STACK *stack_create(void)
{
	STACK *stack = malloc(sizeof(STACK));
	if (!stack)
	{
		perror("malloc failed");
		return NULL;
	}
	stack->stack_ind = 0;  // 初始化栈顶元素
	return stack;
}

// 入栈
int stack_push(STACK *stack, int data)
{
	if (is_full(stack))
		return -1;  // 栈满了

	stack->stack_arr[stack->stack_ind++] = data;  // 入栈
	return 0;
}

// 出栈
int stack_pop(STACK *stack, int *save)
{
	if (is_empty(stack))
		return -1;  // 栈为空

	*save = stack->stack_arr[--stack->stack_ind];  // 出栈
	return 0;
}

// 遍历
void stack_display(STACK *stack)
{
	if (is_empty(stack)) return;  // 栈为空, 直接返回

	for (int i = stack->stack_ind - 1; i >= 0; i--)
		printf("%8d\n", stack->stack_arr[i]);

	printf("--------------------\n\n");
}

// 销毁
void stack_destroy(STACK *stack)
{
	if (stack == NULL)
		return;

	free(stack);
}