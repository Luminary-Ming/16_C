/*
	链栈的实现

	第一个数据节点当作栈顶
	插入 (Push): 在头结点之后插入 (头插法)
	删除 (Pop):  删除头结点之后的第一个节点
*/
#include "stack.h"
#include <stdio.h>

int main(void)
{
	int arr[] = { 11,22,33,44,55,66,77,88,99 };  // 数据源

	STACK *stack = stack_create();  // 创建栈式的存储结构
	if (stack == NULL)
	{
		fprintf(stderr, "stack_create() Is Failed!\n");
		return -1;
	}

	int ret = 0;
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		ret = stack_push(stack, arr[i]);
		if (ret != 0)
		{
			printf("Stack Is Full!\n");
			break;
		}
	}

	stack_display(stack);

	printf("===========================\n");

	int save = 0;
	ret = stack_pop(stack, &save);
	if (ret != 0)
		printf("Stack Is Empty!\n");
	else
		printf("save = %d\n", save);

	printf("===========================\n");

	stack_display(stack);

	stack_destroy(stack);

	return 0;
}