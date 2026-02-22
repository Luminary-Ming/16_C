/*
	顺序栈的实现 - 版本二
*/
#include <stdio.h>
#include "stack.h"

int main(void)
{
	int data[] = { 11,22,33,44,55,66,77,88,99 };  // 数据源

	STACK *stack = stack_create();
	if (stack == NULL)
	{
		fprintf(stderr, "stack_create() Is Failed!\n");
		return -1;
	}

	int ret = 0;  // ret 接收返回值
	for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++)
	{
		ret = stack_push(stack, data[i]);  // 入栈
		if (ret != 0)
		{
			printf("Stack Is Full!\n");  // 打印栈满的信息
			break;
		}
	}

	stack_display(stack);  // 遍历栈

	int save = 0;  // 存储出栈的数据
	ret = stack_pop(stack , &save);
	if (ret != 0)
		printf("Stack Is Empty!\n");
	else
		printf("save = %d\n\n", save);

	stack_display(stack);  // 遍历栈

	stack_push(stack, 9527);  // 入栈

	stack_display(stack);  // 遍历栈

	return 0;
}