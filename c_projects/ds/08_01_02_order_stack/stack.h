#ifndef STACK_H
#define STACK_H

#define STACKSIZE 6  // 栈的大小

typedef struct
{
	int stack_arr[STACKSIZE];  // 栈式的存储空间
	int stack_ind;  // 栈顶元素
}STACK;

STACK *stack_create(void);  // 创建

int stack_push(STACK *stack, int data);  // 入栈

int stack_pop(STACK *stack, int *save);  // 出栈

void stack_display(STACK *stack);  // 遍历

void stack_destroy(STACK *stack);  // 销毁

#endif