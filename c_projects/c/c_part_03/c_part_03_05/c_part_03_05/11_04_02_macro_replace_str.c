/*
   `#`  字符串化运算符, 将宏参数转换为字符串字面量
*/
#include <stdio.h>

#define PRINT_STR(n) #n

int main(void)
{
    printf("PRINT_STR = %s\n", PRINT_STR(9527));
    printf("PRINT_STR = %s\n", PRINT_STR(3.14159265));
    printf("PRINT_STR = %s\n", PRINT_STR(0x12345678));
    printf("PRINT_STR = %s\n", PRINT_STR(123 + 456 - 789));
    printf("PRINT_STR = %s\n", PRINT_STR(你好中国));  // 甚至直接打中文
    printf("PRINT_STR = %s\n", PRINT_STR("Hello World!"));

    return 0;
}
/*
    PRINT_STR = 9527
    PRINT_STR = 3.14159265
    PRINT_STR = 0x12345678
    PRINT_STR = 123 + 456 - 789
    PRINT_STR = 你好中国
    PRINT_STR = "Hello World!"
*/