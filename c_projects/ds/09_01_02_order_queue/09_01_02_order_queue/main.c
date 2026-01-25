#include <stdio.h>
#include "queue.h"

int main(void)
{
    QUEUE *q = NULL;
    q = queue_create();
    if(q==NULL)
    {
        fprintf(stderr, "queue_create Is Failed!\n");
        return -1;
    }

    int arr[] = { 11,22,33,44,55,66,77,88,99 };  // 数据源
    int ret = 0;  // ret 变量用来接收返回值
    for (int i = 0; i < sizeof(arr) / sizeof(*arr); i++)
    {
        ret = queue_en(q, arr[i]);  // 入队
        if (ret != 0)  // 判断入队是否失败
        {
            printf("Queue Is Full!\n");  // 打印队列满的信息
            break;
        }
    }

    queue_display(q);  // 遍历队列

    printf("========================\n");

    int save = 0;  // save 变量用来存储出队的数据
    ret = queue_de(q, &save);//出队
    if (ret != 0)  // 判断出对是否失败
        printf("Queue Is Empty!\n");  // 打印队列为空的信息
    else
        printf("save = %d \n", save);

    printf("========================\n");

    queue_display(q);  // 遍历队列

    printf("========================\n");

    queue_en(q, 9527);  // 入队

    queue_display(q);  // 遍历队列

    return 0;
}
