#include <stdio.h>
#include <stdlib.h>

void trap_example(void)
{
    static int count = 0;
    printf("清理函数调用次数: %d\n", ++count);

    // ❌ 陷阱: 不要在这里调用 exit()
    // exit(0);  // 会导致无限递归调用清理函数
}

int main(void)
{
    atexit(trap_example);

    // ⚠️ 注意: atexit 不能用于异常终止
    // 如果程序被 kill -9 杀死，清理函数不会执行

    return 0;
}
