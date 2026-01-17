/*
    strcat  拼接字符串
*/
#include <stdio.h>
#include <string.h>

int main(void)
{
    char dest[10] = "Hello";
    const char *src = "World";

    // strcat() 函数返回一个指向目标字符串 dest 的指针
    // 将 src 字符串拼接到 dest 后面, 覆盖掉 dest 的 '\0'
    printf("strcat(dest, src) = %s\n",strcat(dest, src));  // strcat(dest, src) = HelloWorld
    printf("dest = %s\n", dest);  // dest = HelloWorld
   
    return 0;
}