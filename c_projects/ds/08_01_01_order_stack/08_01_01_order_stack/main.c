/*
	顺序栈的实现 - 版本一
*/
#include <stdio.h>
#include "stack.h"

int main(void)
{
	int data[] = { 11,22,33,44,55,66,77,88,99 };  // 数据源

	int ret = 0;  // ret 接收返回值
	for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++)
	{
		ret = stack_push(data[i]);
		if (ret != 0)
		{
			printf("Stack Is Full!\n");  // 打印栈满的信息
			break;
		}
	}

	stack_display();  // 遍历栈

	int save = 0;  // 存储出栈的数据
	ret = stack_pop(&save);  // 出栈
	if (ret != 0)
		printf("Stack Is Empty!\n");
	else
		printf("save = %d\n\n", save);

	stack_display();  // 遍历栈

	stack_push(9527);  // 入栈

	stack_display();  // 遍历栈

	return 0;
}