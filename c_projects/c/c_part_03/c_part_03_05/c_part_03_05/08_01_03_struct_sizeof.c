/*
    结构体通过 sizeof 计算大小
*/
#include <stdio.h>
#include <string.h>

struct stu
{
    int id;
    char name[20];
    int math;
    double score;
};

int main(void)
{
    struct stu s;

    printf("sizeof(s) = %ld\n", sizeof(s));  // 40
    printf("sizeof(struct stu) = %ld\n", sizeof(struct stu));  // 40
    printf("sizeof(s.id) = %ld\n", sizeof(s.id));  // 4
    printf("sizeof(s.name) = %ld\n", sizeof(s.name));  // 20
    printf("sizeof(s.math) = %ld\n", sizeof(s.math));  // 4
    printf("sizeof(s.double) = %ld\n", sizeof(s.score));  // 8

    printf("s       = %p\n", s);        // 0x7fffffffe9d8
    printf("s.id    = %p\n", &s.id);    // 0x7fffffffe890  90 ~ 94 占 4  个字节
    printf("s.name  = %p\n", s.name);   // 0x7fffffffe894  94 ~ a8 占 20 个字节
    printf("s.mat   = %p\n", &s.math);  // 0x7fffffffe8a8  a8 ~ b0 占 8  个字节
    printf("s.score = %p\n", &s.score); // 0x7fffffffe8b0  b0 ~ b8 占 8  个字节

    return 0;
}
/*
    结构体对齐规则
    1.所有成员中对齐要求的最大值 = max(4, 1, 4, 8) = 8
    2.每个成员的起始偏移必须是其对齐值的倍数
    3.结构体总大小必须是结构体对齐值（8）的倍数

    分析
    int id：大小 4 字节，对齐要求 4
    char name[20]：大小 20 字节，对齐要求 1（因为 char 对齐要求 1）
    int math：大小 4 字节，对齐要求 4
    double score：大小 8 字节，对齐要求 8
    所以对齐要求的最大值是 8

    int id         大小4 
    char name[20]  大小20
    4 + 20 = 24, 加起来正好是 8 的倍数, 所以分配 3 个 8 字节的空间放 int id 和 char name[20]
    int math       大小4, 所以要分配 1 个 8 字节的空间放 int math
    double score   大小8, 正好分配 1 个 8 字节的空间放 double score

    24 + 8 + 8 = 40
*/