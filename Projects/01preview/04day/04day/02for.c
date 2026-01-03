#include <stdio.h>
/*
	for 循环
*/
int main()
{
	int sum = 0;
	for (int i = 1; i < 101; i++) {
		sum += i;
	}
	printf("1到100的和是: %d\n", sum);

	return 0;
}