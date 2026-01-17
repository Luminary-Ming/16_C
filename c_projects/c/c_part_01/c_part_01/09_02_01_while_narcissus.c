/*
    while 循环
    
    水仙花数
*/

#include <stdio.h>

int main(void)
{
    int b = 1, s = 0, g = 0;  // 分别表示百位 十位 个位

    while (b < 10)
    {
        s = 0; // 重置, 赋值 0 重新遍历, 不然每个内层循环只会执行一次
        while (s < 10)
        {
            g = 0;
            while (g < 10)
            {
                if (b * b * b + s * s * s + g * g * g == b * 100 + s * 10 + g)
                    printf("%d\n", b * 100 + s * 10 + g);
                g++;
            }
            s++;
        }
        b++;
    }

    return 0;
}