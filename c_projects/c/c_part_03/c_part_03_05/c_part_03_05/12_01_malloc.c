#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p = NULL;  // 使用指针 p 指向开辟的空间

    p = malloc(sizeof(int));  // 开辟 4byte 的空间
    if (p == NULL)  // 判断开辟空间是否失败
    {
        fprintf(stderr, "malloc() Is Failed!\n");  //打印错误信息
        return -1;  // 由于开辟空间失败,结束程序,并且返回 -1
    }

    *p = 9527;  // 通过指针p访问开辟的空间

    printf("*p = %d\n", *p);

    free(p);   // 释放开辟的空间
    p = NULL;  // 为了避免使用野指针, 把野指针转变为空指针

    return 0;
}