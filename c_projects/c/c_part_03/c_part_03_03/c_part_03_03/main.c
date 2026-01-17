#include <stdio.h>
#include "swap.h"

int main(void)
{
    int a = 0, b = 0;

    scanf("%d %d", &a, &b);

    printf("a = %d b = %d\n", a, b);

    swap(&a, &b);

    printf("a = %d b = %d\n", a, b);

    return 0;
}