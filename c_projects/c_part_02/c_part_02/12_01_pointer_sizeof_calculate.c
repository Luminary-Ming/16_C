/*
	通过 sizeof() 运算符计算指针所占字节大小
*/
#include <stdio.h>

int main(void)
{
    printf("sizeof(char *) = %ld\n", sizeof(char *)); 
    printf("sizeof(short *) = %ld\n", sizeof(short *));  
    printf("sizeof(int *) = %ld\n", sizeof(int *));
    printf("sizeof(long *) = %ld\n", sizeof(long *));
    printf("sizeof(float *) = %ld\n", sizeof(float *));
    printf("sizeof(double *) = %ld\n", sizeof(double *));
    printf("sizeof(void *) = %ld\n", sizeof(void *));

    return 0;
}
/*
    sizeof(char *) = 8
    sizeof(short *) = 8
    sizeof(int *) = 8
    sizeof(long *) = 8
    sizeof(float *) = 8
    sizeof(double *) = 8
    sizeof(void *) = 8
*/