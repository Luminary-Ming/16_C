#include "stack.h"
#include "llist.h"
#include <stdio.h>

STACK *stack_create(void)
{
	return llist_create();
}

static int is_full(STACK *s)  // 链栈永远不会存满
{
	return 0;
}

static int is_empty(STACK *s)  // 链栈是否为空
{
	return llist_is_empty(s);
}

int stack_push(STACK *s, int data)
{
	if (is_full(s))
		return -1;
	llist_insert(s, &data, TAILINSERT);
	return 0;
}

int stack_pop(STACK *s, int *save)
{
	if (is_empty(s))
		return -1;
	llist_pop(s, save);
	return 0;
}

void stack_display(STACK *s)
{
	if (is_empty(s))  // 链栈是空的, 直接返回
		return;
	llist_display(s);
}

void stack_destroy(STACK *s)
{
	if (s == NULL)  // 链栈是 NULL, 直接返回
		return;
	llist_destroy(s);
}