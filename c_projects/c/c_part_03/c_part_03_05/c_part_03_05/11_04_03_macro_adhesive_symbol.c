/*
    `##`  记号黏合运算符, 连接两个预处理记号
*/
#include <stdio.h>

#define PRINT_VAR(n) printf("var" #n " = %d\n", var##n)

int main(void)
{
    int var1 = 100;
    int var2 = 200;
    int var3 = 300;
    int var4 = 400;
    int var5 = 500;
    
    PRINT_VAR(1);  // 替换为 printf("var" "1" "= %d\n", var1)
    PRINT_VAR(2);
    PRINT_VAR(3);
    PRINT_VAR(4);
    PRINT_VAR(5);
    return 0;
}
/*
    var1 = 100
    var2 = 200
    var3 = 300
    var4 = 400
    var5 = 500
*/