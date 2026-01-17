/*
	字符串函数
	strcpy  复制字符串
*/
#include <stdio.h>
#include <string.h>

int main(void)
{
    char dest[10] = "Hello";
    char *src = "World";

    // strcpy(dest, src) 函数返回的指针就是 dest, 所以这俩的值相同
    printf("strcpy = %s\n", strcpy(dest, src));  // strcpy = World
    printf("dest = %s\n", dest);  // dest = World

    //strcpy(src, dest);  // 不可以, 因为 src 指向常量区

    printf("strcpy = %s\n", strcpy(dest, "Easthome"));  // strcpy = Easthome
    printf("dest = %s\n", dest);  // dest = Easthome

    //char test[10] = "abcde";
    ////strcpy(dest, test);
    ////第一个参数必须是字符数组,第二个参数可以是任意形式的字符串
    //printf("strcpy = %s\n", strcpy(dest, src));

    //printf("dest = %s\n", dest);

    return 0;
}