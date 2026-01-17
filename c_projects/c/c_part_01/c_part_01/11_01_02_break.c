/*
    break 在嵌套循环语句, 只能跳出最近的循环体
*/
#include <stdio.h>

int main(void)
{
    int i = 0, j = 0;

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (i == 2 || j == 2) break;
            printf("(%d,%d) ", i, j);
        }
        printf("\n");
    }

    return 0;
}
/*
    (0,0) (0,1)
    (1,0) (1,1)

    (3,0) (3,1)
    (4,0) (4,1)
*/