#include "stack.h"
#include <stdlib.h>

STACK *stack_create(void)
{
	return llist_create();
}

static int is_full(STACK *stack)
{
	return 0;  // 链栈永远不会满
}

static int is_empty(STACK *stack)
{
	return llist_is_empty(stack);   // 链栈是否为空
}

int stack_push(STACK *stack, int data)
{
	if (is_full(stack)) return -1;
	
	return llist_insert(stack, &data, HEADINSERT);  // 链栈用头插
}

int stack_pop(STACK *stack, int *save)
{
	if (is_empty(stack)) return -1;

	return llist_pop(stack, save);
}

void stack_display(STACK *stack)
{
	if (is_empty(stack)) return;

	llist_display(stack);
}

void stack_destroy(STACK *stack)
{
	if (stack == NULL) return;

	llist_destory(stack);
}