#ifndef __cplusplus
#error "对不起, 现在使用的不是 C++ 的编译器"
#endif

#include <stdio.h>

int main(void)
{
	printf("Hello World!\n");
}
/*
	可以不加引号：对于简单英文警告消息
	建议加引号：对于中文、包含标点、特殊字符的消息
	加引号更安全：保证在所有编译器上正常工作
	一句话：加引号总是对的，不加引号可能在某些情况下有问题。为了代码健壮性，建议总是加双引号
*/