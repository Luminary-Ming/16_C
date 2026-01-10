/*
	do while 循环

    至少执行一次循环
*/
#include <stdio.h>

int main(void)
{
    int i = 1;  // 循环变量
    int sum = 0;  // 存储计算的总和

    do
    {
        sum += i++;
    } while (i <= 100);

    printf("sum = %d\n", sum);

    return 0;
}