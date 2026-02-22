#include"stack.h"
#include <stdio.h>

static int stack_arr[STACKSIZE];  // 使用全局数组实现栈式存储结构
static int stack_ind;  // 使用全局变量作为栈顶元素的标记(默认标记为 0)

// 栈是是否满了
static int is_full(void)
{
	return stack_ind == STACKSIZE;
}

// 栈是否是空栈
static int is_empty(void)
{
	return stack_ind == 0;
}

// 入栈
int stack_push(int data)
{
	if (is_full())
		return -1;  // 栈满了

	stack_arr[stack_ind++] = data;  // 入栈
	return 0;
}

// 出栈
int stack_pop(int *save)
{
	if (is_empty())
		return -1;  // 栈为空

	*save = stack_arr[--stack_ind];  // 出栈
	return 0;
}

// 遍历
void stack_display(void)
{
	if (is_empty()) return;  // 栈为空, 直接返回

	for (int i = stack_ind - 1; i >= 0; i--)
		printf("%8d\n", stack_arr[i]);

	printf("--------------------\n\n");
} 