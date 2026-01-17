#include <stdio.h>

#define SUB  // 定义名为 SUB 的宏
#define STM32F103C8T6  // 定义名为 STM32F103C8T6 的宏

int main(void)
{
    int a = 13, b = 7;
#ifdef SUB  // 如果定义了 SUB 宏
    printf("%d + %d = %d\n", a, b, a + b);  // 13 + 7 = 20
#else
    printf("%d - %d = %d\n", a, b, a - b);
#endif  // 条件编译块结束


#ifndef ADD  // 如果没有定义 ADD 宏
    printf("%d * %d = %d\n", a, b, a * b);  // 13 * 7 = 91
#else
    printf("%d / %d = %d\n", a, b, a / b);
#endif  // 条件编译块结束


    int PIN = 0;  // 表示芯片的管脚
#if defined(STM32F103C8T6)  // 相当于 #ifdef
    PIN = 64;
#elif defined(STM32F103C8T6)
    PIN = 32;
#endif  // 条件编译块结束
    printf("PIN = %d\n", PIN);  // 64

    return 0;
}