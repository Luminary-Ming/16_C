/*
	输入10个数，分别统计其中正数、负数、零的个数。
*/
#include <stdio.h>

int main()
{
	double arr[10];
	int positive_count = 0, negative_count = 0, zero_count = 0;

	for (int i = 0; i < 10; i++) {
		scanf("%lf", &arr[i]);
	}

	for (int i = 0; i < 10; i++) {
		if (arr[i] > 0) {
			positive_count++;
		}
		else if (arr[i] < 0) {
			negative_count++;
		}
		else {
			zero_count++;
		}
	}

	printf("正数的个数是 %d\n", positive_count);
	printf("负数的个数是 %d\n", negative_count);
	printf("零的个数是 %d\n", zero_count);

}