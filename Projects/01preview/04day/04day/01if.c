#include <stdio.h>
/*
    if 语句

    在C语言中，if 语句的条件判断遵循以下规则：
    0 为假（false）
    任何非 0 值都为真（true）

*/
int main()
{
    printf("测试不同if条件的真假：\n");

    if (1) printf("1 为真\n");
    if (-1) printf("-1 为真\n");
    if (0) printf("0 为真\n");  // 0 为假, 不会打印
    if ("a") printf("\"a\" 为真\n");
    if ("hello") printf("\"hello\" 为真\n");
    if ("") printf("\"\" 为真\n");  // 空字符串也为真！
    if (NULL) printf("NULL 为真\n");  // NULL 相当于 0 , 为假, 不会打印


    // 验证地址值
    printf("\n字符串地址：%p\n", "a");
    printf("空字符串地址：%p\n", "");

    return 0;
}