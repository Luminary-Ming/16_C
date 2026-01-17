#include <stdio.h>

void func(int p[5])
{
    int i = 0;
    printf("p = %p\n", p);
    p++;
    printf("p = %p\n", p);
    p += 1;
    printf("p = %p\n", p);
    printf("sizeof(p) = %ld\n", sizeof(p));
    p = &i;
}

int main(void)
{
    int arr[5] = { 11,22,33,44,55 };

    func(arr);

    return 0;
}
/*
    p = 0x7fffffffe8e0
    p = 0x7fffffffe8e4
    p = 0x7fffffffe8e8
    sizeof(p) = 8
*/