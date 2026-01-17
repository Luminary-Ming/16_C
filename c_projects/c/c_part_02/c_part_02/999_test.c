/*
*/
#include <stdio.h>

int main(void)
{
	//char str[3];
	char *p = "hello";

	printf("%d %d", sizeof(p), sizeof(*p));

	return 0;
}