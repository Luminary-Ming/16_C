/*
	#pargma pack
	3. 控制内存对齐
*/
#include <stdio.h>

//#pragma pack(3)   // 不生效, 按默认 int 4字节对齐
//#pragma pack(19)  // 不生效, 按默认 int 4字节对齐
#pragma pack(2)  // 2 字节对齐
struct aaa
{
    char c;
    int i;
};
#pragma pack()

int main(void)
{
    printf("sizeof(struct aaa) = %ld\n", sizeof(struct aaa));  // 6

    return 0;
}
/*
    #pragma pack(3)
    #pragma pack(19)
    大多数编译器对 #pragma pack(n) 的 n 值有限制：
    有效值：通常是 1, 2, 4, 8, 16（2的幂次）
    奇数值：编译器通常会忽略或调整为有效值
    过大值：会被限制为编译器支持的最大对齐值
*/