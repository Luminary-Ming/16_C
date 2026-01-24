#ifndef __STACK_H
#define __STACK_H

#include "llist.h"

typedef LLIST STACK;

STACK *stack_create(void);

int stack_push(STACK *, int);

int stack_pop(STACK *, int *);

void stack_display(STACK *);

void stack_destroy(STACK *);

#endif