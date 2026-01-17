#include <stdio.h>

//#define ARRSIZE 10

int main(void)
{
#if !defined(ARRSIZE)
#error "ARRSIZE Is Not Defined!"
#else
	int arr[ARRSIZE];
#endif
	return 0;
}