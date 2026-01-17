/*
	标准格式化 - 宽度和精度
*/
#include <stdio.h>

int main(void)
{
    // 右对齐 10 字符 ( 所以 200 前面补了 7 个空字符 )
    printf("%10d\n", 200);  //       200

    // 保留两位小数
    printf("%.2f\n", 3.1415926535);  // 3.14

    // 保留前 5 个字符
    printf("%.5s\n", "easthome");  // easth

    // 右对齐 10 字符, 同时保留两位小数
    printf("%10.2f\n", 3.14159);  //       3.14

    return 0;
}