#include <stdio.h>
#include <unistd.h>
#include "16_anytimer.h"

void my_handler(void *arg)
{
    int *num = (int *)arg;
    printf("闹钟触发，参数: %d\n", *num);
}

int main()
{
    int a = 100, b = 200, c = 300;
    int td1 = anytimer_init(1, my_handler, &a);
    int td2 = anytimer_init(3, my_handler, &b);
    int td3 = anytimer_init(5, my_handler, &c);

    printf("闹钟描述符: %d, %d, %d\n", td1, td2, td3);

    while (1)
        pause();  // 等待信号
    return 0;
}
/*
    闹钟描述符: 0, 1, 2
    闹钟触发，参数: 100 (1s 后触发)
    闹钟触发，参数: 200 (3s 后触发)
    闹钟触发，参数: 300 (5s 后触发)
    (所有闹钟执行完毕后, 程序进入无限循环的 pause() 等待信号, 手动按 Ctrl+C 终止程序)
*/