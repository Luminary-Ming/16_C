/*
    switch 语句

    atoi() 字符串数字转整数
*/
#include <stdio.h>

int main(void)
{
    char *p = "9527";
    int num = 0;

    for (; *p != '\0'; p++)  // 可以遍历到所有的有效字符
    {
        switch (*p)
        {
            case '0' ... '9': num *= 10; num += *p - '0'; break;  // 内核写法
            default: break;
        }
    }

    printf("num = %d\n", num);  // num = 9527

    return 0;
}