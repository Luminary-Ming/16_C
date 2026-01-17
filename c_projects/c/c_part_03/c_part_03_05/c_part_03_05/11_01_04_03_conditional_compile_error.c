#include <stdio.h>

#define ARRSIZE 10

int main(void)
{
#if ARRSIZE < 20
#error "ARRSIZE Is Small!"
#else
    int arr[ARRSIZE];
#endif

    return 0;