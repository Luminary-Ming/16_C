/*
	标准格式化 - 长度修饰
*/
#include <stdio.h>

int main(void)
{
    char c = 65;
    short s = 12345;
    int i = 123456789;
    long l = 1234567890L;
    long long ll = 123456789012345LL;
    // size_t 无符号整数类型, 专门用于表示对象大小、数组索引、循环计数等非负值的类型
    size_t size = sizeof(int);

    printf("char (%%hhd):     %hhd\n", c);   // char (%hhd):     65
    printf("char (%%c):       %c\n", c);     // char (%c):       A      
    printf("short (%%hd):     %hd\n", s);    // short (%hd):     12345  
    printf("int (%%d):        %d\n", i);     // int (%d):        123456789
    printf("long (%%ld):      %ld\n", l);    // long (%ld):      1234567890
    printf("long long (%%lld):%lld\n", ll);  // long long (%lld):123456789012345
    printf("size_t (%%zu):    %zu\n", size); // size_t (%zu):    4
    
    return 0;
}