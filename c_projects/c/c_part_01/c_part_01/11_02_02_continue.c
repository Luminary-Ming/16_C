/*
    continue 在循环体中是跳出本次循环, 进入下一次循环
    continue 在嵌套循环体中只能跳出 continue 所在的循环体中
*/
#include <stdio.h>

int main(void)
{
    int i = 0, j = 0;

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (i == 2 || j == 2)
                continue;
            printf("(%d,%d) ", i, j);
        }
        printf("\n");
    }

    return 0;
}
/*
    (0,0) (0,1) (0,3) (0,4)
    (1,0) (1,1) (1,3) (1,4)

    (3,0) (3,1) (3,3) (3,4)
    (4,0) (4,1) (4,3) (4,4)
*/