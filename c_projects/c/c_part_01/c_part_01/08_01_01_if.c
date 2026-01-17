/*
    if 语句
*/
#include <stdio.h>

int main(void)
{
    int year = 0;

    scanf("%d", &year);

    // 是闰年  ->  year % 400 = 0  ->  !(year % 400) = 1
    if ((!(year % 4) && year % 100) || !(year % 400)) {
        printf("%d年是闰年!\n", year);
    }
    else {
        printf("%d年是平年!\n", year);
    }

    return 0;
}