#include <stdio.h>
#include "swap.h"
#include "max.h"
#include "add.h"

int main(void)
{
    int a = 0, b = 0;

    scanf("%d %d", &a, &b);

    printf("a = %d b = %d\n", a, b);

    swap(&a, &b);

    printf("a = %d b = %d\n", a, b);

    printf("max = %d\n", max(a, b));

    printf("a + b = %d\n", add(a, b));

    return 0;
}