/*
    mystrlen  获取字符串长度 (不包括 '\0')
*/
#include <stdio.h>
#include <string.h>

size_t mystrlen1(const char* str);
size_t mystrlen2(const char *str);

int main(void)
{
    char dest[10] = " Hello ";  // 开头结尾都有一个空格
    const char *src = "Wo rld";  // 中间有一个空格

    printf("dest = %d\n", mystrlen2(dest));  // dest = 7
    printf("src = %d\n", mystrlen2(src));  // src = 6 

    return 0;
}

size_t mystrlen2(const char *str)
{
    const char *save = str;
    while (*++str);
    return save - str;
}

size_t mystrlen1(const char *str)
{
    int i = 0;
    for (i = 0; str[i] != '\0'; i++);
    return i;
}