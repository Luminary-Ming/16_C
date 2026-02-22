#ifndef STACK_H
#define STACK_H

#define STACKSIZE 6  // 栈的大小

int stack_push(int data);  // 入栈

int stack_pop(int *save);  // 出栈

void stack_display(void);  // 遍历

#endif