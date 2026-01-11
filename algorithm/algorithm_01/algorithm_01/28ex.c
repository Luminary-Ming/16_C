/*
	输入 n, 随机产生 n 个两位自然数，降序排列后输出。
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(void)
{
	int n;
	scanf("%d", &n);

	int arr[n];

	srand(getpid() + time(NULL));  // 随机种子

	// 随机产生 n 个两位自然数
	for (int i = 0; i < n; i++) {
		int number = rand() % 90 + 10;
		arr[i] = number;
	}

	// 冒泡排序
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1 - i; j++) {
			if (arr[j] < arr[j+1]) {
				int temp = arr[j + 1];
				arr[j + 1] = arr[j];
				arr[j] = temp;
			}
		}
	}

	printf("降序排列后:\n");
	
	for (int i = 0; i < n; i++) {
		printf("%d\n", arr[i]);
	}

	return 0;
}