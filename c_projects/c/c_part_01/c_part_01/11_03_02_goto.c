/*
    尽量不要使用 goto 
*/
#include <stdio.h>

int main(void)
{
    if (1){
        printf("Hello!\n");
        goto flag;
    }
    else {
    flag:
        printf("World!\n");
    }

    return 0;
}
/*
    goto flag 跳过了 else 的条件检查, 
    直接执行了 else 块内的代码，破坏了程序的结构逻辑。
*/