/*
    strcmp  比较字符串内容
*/
#include <stdio.h>
#include <string.h>

int main(void)
{
    char dest[10] = "Hello";
    const char *src = "World";

    // dest 的第一个字符是 'H'  ASCII值是 72
    // src  的第一个字符是 'W'  ASCII值是 87
    // 'H' - 'W' = 72 - 87 = -15
    printf("strcat(dest, src) = %d\n", strcmp(dest, src));  // strcat(dest, src) = -15

    dest[0] = 'W';  // 第一个字符换成相同的 'W'
    // 第一个字符相同, 开始比较第二个字符
    // 'e' - 'o' = 101 - 111 = -10
    printf("strcat(dest, src) = %d\n", strcmp(dest, src));  // strcat(dest, src) = -10

    dest[0] = 'H';  // 换回来
    src = "Hello";  // 让 src 指向一个新的字符串, 和 dest 的字符串内容相同
    // 逐个字符比较, 全部相同返回 0
    printf("strcat(dest, src) = %d\n", strcmp(dest, src));  // strcat(dest, src) = 0

    return 0;
}