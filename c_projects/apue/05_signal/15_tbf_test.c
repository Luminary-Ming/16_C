#include <stdio.h>
#include <unistd.h>
#include "15_tbf.h"

int main()
{
    int td1 = tbf_init(10, 50);  // 速率 10 个/秒，容量 50
    int td2 = tbf_init(5, 20);   // 速率  5 个/秒，容量 20
    printf("桶描述符: td1=%d, td2=%d\n", td1, td2);

    // 尝试取令牌
    int fetch;
    fetch = tbf_fetch_token(td1, 15);
    printf("取15个，实际取到 %d 个\n", fetch);

    sleep(2);  // 等待 2s, 形成时间差, 为了补充令牌

    fetch = tbf_fetch_token(td1, 100);
    printf("取100个，实际取到 %d 个\n", fetch);

    fetch = tbf_fetch_token(td2, 12);
    printf("从桶2取12个，实际取到 %d 个\n", fetch);

    // 销毁桶
    tbf_destroy(td1);
    tbf_destroy(td2);

    return 0;
}