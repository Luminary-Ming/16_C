/*
	mystrcat  拼接字符串
*/
#include <stdio.h>
#include <string.h>

char *mystrcat(char *dest, const char *src);

int main(void)
{
    char dest[10] = "Hello";
    const char *src = "World";

    // 将 src 字符串拼接到 dest 后面, 覆盖掉 dest 的 '\0'
    printf("strcat(dest, src) = %s\n", mystrcat(dest, src));  // mystrcat(dest, src) = HelloWorld
    printf("dest = %s\n", dest);  // dest = HelloWorld

    return 0;
}

char *mystrcat(char *dest, const char *src) 
{
    int i = 0, j = 0;  // 循环变量

    for (i = 0; dest[i] != '\0'; i++);  // 找出 dest '\0' 的位置
   
    for (j = 0; src[j] != '\0'; j++)
    {
        dest[i + j] = src[j];  // 拼接有效字符串
    }
    dest[i + j] = src[j];  // 拼接 '\0'

    return dest;
}