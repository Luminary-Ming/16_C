/*
    避免野指针
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p = NULL;

    p = malloc(sizeof(int));  // 开辟 4 字节的堆区空间
    if (p == NULL)  // 判断是否开辟失败
    {
        printf("开辟空间失败!\n");  //打印错误信息
        return -1;  // 由于开辟空间失败, 结束程序, 并且返回 -1
    }

    *p = 9527;

    printf("free 之前  p = %p\n", p);  // free 之前  p = 0x5555555592a0
    printf("free 之前 *p = %d\n", *p);  // free 之前 *p = 9527

    free(p);  // 释放开辟的空间 (只释放内存，不改变指针的值)
    p = NULL;  // 防止使用野指针

    *p = 6666;  // p 变为了空指针, 不能给空指针赋值, 会报段错误
}
