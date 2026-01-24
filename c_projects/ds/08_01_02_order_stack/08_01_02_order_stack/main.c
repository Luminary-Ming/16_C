/*
	顺序栈的实现 - 版本二
*/
#include "stack.h"
#include <stdio.h>

int main(void)
{
	STACK *s = NULL;  // s 指向栈式的存储结构
	int arr[] = { 11,22,33,44,55,66,77,88,99 };  // 数据源

	s = stack_create();  // 创建栈式的存储结构
	if (s == NULL)
	{
		fprintf(stderr, "stack_create() Is Failed!\n");
		return -1;
	}

	int ret = 0;
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		ret = stack_push(s, arr[i]);   // 入栈
		if (ret != 0)
		{
			printf("Stack Is Full!\n");
			break;
		}
	}

	stack_display(s);   // 遍历栈

	int save = 0;  // 存储出栈数据
	ret = stack_pop(s, &save);
	if(ret !=0)
		printf("Stack Is Empty!\n");
	else
		printf("save = %d\n\n", save);

	stack_display(s);  // 遍历栈

	stack_push(s, 9527);  // 入栈

	stack_display(s);  // 遍历栈

	stack_destroy(s);  // 销毁栈

	return 0;
}