#include <stdio.h>
/*
    自增运算符
    对变量进行加1操作
    ++变量  : 先自增1, 再参与表达式运算
    变量++  : 先参与表达式运算, 再自增1
*/
int main()
{
    char c = 'A';
    int i = 9527;
    float f = 3.14;

    printf("c = %c i = %d f = %f\n", c, i, f);  // c = A i = 9527 f = 3.140000

    c++;
    i++;
    f++;

    printf("c = %c i = %d f = %f\n", c, i, f);  // c = B i = 9528 f = 4.140000

    ++c;
    ++i;
    ++f;

    printf("c = %c i = %d f = %f\n", c, i, f);  // c = C i = 9529 f = 5.140000

    return 0;
}