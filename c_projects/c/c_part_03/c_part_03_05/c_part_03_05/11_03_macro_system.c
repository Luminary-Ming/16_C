/*
    系统已定义的宏
*/
#include <stdio.h>

int main(void)
{
    printf("__DATE__ = %s\n", __DATE__);  // 编译日期（"Mmm dd yyyy" 格式字符串）
    printf("__TIME__ = %s\n", __TIME__);  // 编译时间（"hh:mm:ss" 格式字符串）
    printf("__FILE__ = %s\n", __FILE__);  // 当前源文件的文件名（字符串）
    printf("__LINE__ = %d\n", __LINE__);  // 当前行号（十进制整数）
    printf("__FUNCTION__ = %s\n", __FUNCTION__);  // 当前函数名

    return 0;
}
/*
__DATE__ = Jan 15 2026
__TIME__ = 19:32:33
__FILE__ = /home/xinming/easthome/c_projects/c_part_03/c_part_03_05/11_03_macro_system.c
__LINE__ = 8
__FUNCTION__ = main
*/