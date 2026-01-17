#include <stdio.h>
/*
    条件运算符
*/
int main()
{
    int a = 0, b = 0;
    int max = 0;  //max变量存储a和b中的最大值

    scanf("%d %d", &a, &b);

    /*
        如果 a > b 为真, max = printf("a = %d\n", a);
        如果 a > b 为假, max = printf("b = %d\n", b);
        max 变量并不是获取的 a 和 b 的值,而是 printf(3) 的返回值

        printf() 函数的返回值是：
         · 成功时：返回打印的字符总数（包括数字、字母、空格、换行符等所有字符）
         · 失败时：返回负值
    */
    max = a > b ? printf("a = %d\n", a) : printf("b = %d\n", b);

    printf("max = %d\n", max);
    /*
        输入 13 7
        输出 a = 13
             max = 7
        "a = 13\n" 有 7 个字符
    */
    return 0;
}