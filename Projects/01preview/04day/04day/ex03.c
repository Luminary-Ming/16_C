/*
    需求 : 录入三角形的三个边,判断是否可以构成普通三角形,是否可以构成直角三角形,是否可以构成等腰三角形,是否可以构成等边三角形(要求只会打印出一种结果)
*/
#include <stdio.h>

int main()
{
    int a = 0, b = 0, c = 0;//分别表示三角形的三个边

    scanf("%d %d %d", &a, &b, &c);//通过scanf(3)录入三角形的三个边

    /*if error*///判断录入的a b c是否大于0(判断录入的数值是否合理)

    if ((a + b > c) && (a + c > b) && (b + c > a)) {

        if ((a * a + b * b == c * c) || (a * a + c * c == b * b) || (b * b + c * c == a * a)) {
            printf("可以构成直角三角形!\n");
        }
        else if ((a == b) || (a == c) || (b == c)) {
            if (a == b && a == c)
                printf("可以构成等边三角形!\n");
            else
                printf("可以构成等腰三角形!\n");
        }
        else
            printf("可以构成普通三角形!\n");

    }
    else {
        printf("不能构成三角形!\n");
    }

    return 0;
}