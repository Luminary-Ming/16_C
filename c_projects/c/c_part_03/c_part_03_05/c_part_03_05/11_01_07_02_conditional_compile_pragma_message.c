/*
	#pargma message
	2. 编译时显示消息
*/
#include <stdio.h>

#pragma message("Compling" __FILE__)  // 编译时显示消息

int main(void)
{
    printf("Hello World!\n");

    return 0;
}