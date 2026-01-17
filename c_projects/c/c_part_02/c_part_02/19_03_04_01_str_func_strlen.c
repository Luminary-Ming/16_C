/*
    strlen  获取字符串长度 (不包括 '\0')
*/
#include <stdio.h>
#include <string.h>

int main(void)
{
    char dest[10] = " Hello ";  // 开头结尾都有一个空格
    const char *src = "Wo rld";  // 中间有一个空格

    printf("dest = %d\n", strlen(dest));  // dest = 7
    printf("src = %d\n", strlen(src));  // src = 6 

    return 0;
}