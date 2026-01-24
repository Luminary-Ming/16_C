#include <stdio.h>
#include <string.h>


int main(void)
{
	char a[] = "hello\0world";
	char b[15] = { 0 };
	memcpy(b, a, 12);
	printf("strlen(a) = %d\n", strlen(a));  // 5
	printf("strlen(b) = %d\n", strlen(b));  // 5
	printf("sizeof(a) = %d\n", sizeof(a));  // 12
	printf("sizeof(b) = %d\n", sizeof(b));  // 15
	return 0;
}

