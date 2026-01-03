#include <stdio.h>
/*
    赋值运算符  :  把等号右边的项经过计算赋给左边的项
*/
int main()
{

    int a = 0;

    a += 20;  //a = a + 20;
    printf("a = %d\n", a);

    a -= 10;  //a = a - 10;
    printf("a = %d\n", a);

    a *= 3;  //a = a * 3;
    printf("a = %d\n", a);

    a /= 10; //a = a / 10;
    printf("a = %d\n", a);

    a *= 6 + 4;  //a = a * (6 + 4);
    printf("a = %d\n", a);


    return 0;
}