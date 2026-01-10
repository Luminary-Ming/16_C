/*
    goto 功能是跳到拥有标识符的位置

    [1] 在同一个代码结构中标识符不能重复
    [2] 标识符的命名规则和变量名的命名规则相同
    [3] goto 不能跨函数跳跃

*/

#include <stdio.h>

int main(void)
{
    int i = 1;
    int sum = 0;

loop:
    if (i <= 100)
    {
        sum += i++;
        goto loop;
    }

    printf("sum = %d\n", sum);

    return 0;
}


// [3] goto 不能跨函数跳跃
void func1() {
label1:
    printf("func1\n");
}

void func2() {
    // goto label1;  // 错误：试图跳到另一个函数
}