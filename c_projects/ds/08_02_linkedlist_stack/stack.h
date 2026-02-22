#ifndef STACK_H
#define STACK_H

#include "llist.h"

typedef LLIST STACK;

STACK *stack_create(void);

int stack_push(STACK *stack, int data);

int stack_pop(STACK *stack, int *save);

void stack_display(STACK *stack);

void stack_destroy(STACK *stack);

#endif