#include <stdio.h>

#define VERSION 0

int main(void)
{
#if VERSION < 1
#warning "版本过低, 建议升级到 1.0 版本以上"
	int arr[10];
#else
	int arr[20];
#endif
	printf("sizeof(arr) = %ld\n", sizeof(arr));
	return 0;
}
/*
	可以不加引号：对于简单英文警告消息
	建议加引号：对于中文、包含标点、特殊字符的消息
	加引号更安全：保证在所有编译器上正常工作
	一句话：加引号总是对的，不加引号可能在某些情况下有问题。为了代码健壮性，建议总是加双引号
*/