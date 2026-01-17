/*
	大端格式和小端格式
*/
#include <stdio.h>

union U
{
    int i;
    char c[4];
};

int main(void)
{
    union U u;

    u.i = 13;

    printf("u.c[0] = %d &u.c[0] = %p\n", u.c[0], &u.c[0]);
    printf("u.c[1] = %d &u.c[1] = %p\n", u.c[1], &u.c[1]);
    printf("u.c[2] = %d &u.c[2] = %p\n", u.c[2], &u.c[2]);
    printf("u.c[3] = %d &u.c[3] = %p\n", u.c[3], &u.c[3]);

    return 0;
}
/*
    u.c[0] = 13 &u.c[0] = 0x7fffffffe8b4
    u.c[1] = 0  &u.c[1] = 0x7fffffffe8b5
    u.c[2] = 0  &u.c[2] = 0x7fffffffe8b6
    u.c[3] = 0  &u.c[3] = 0x7fffffffe8b7
*/