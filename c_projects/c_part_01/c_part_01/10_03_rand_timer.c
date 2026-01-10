#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
    计时器
*/

int main()
{
    int hour = 0, minute = 0, second = 0;

    for (hour = 0; hour < 24; hour++)
    {
        for (minute = 0; minute < 60; minute++)
        {
            for (second = 0; second < 60; second++)
            {
                system("clear");//清屏

                printf("\33[05;20H%02d : %02d : %02d\n", hour, minute, second);
                sleep(1);  // 休眠 1 s
            }
        }
    }

    return 0;
}