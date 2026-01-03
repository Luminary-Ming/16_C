/*
	随机产生100个三位自然数输出，然后再输出其中同时是3、5、7倍数的数
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
	int n;

	srand(time(NULL) + getpid());  // 随机种子

	for (int i = 0; i < 100; i++) {
		n = rand() % 900 + 100;  // 100 - 999

		if (n % 3 == 0 && n % 5 == 0 && n % 7 == 0) {
			printf("%d\n", n);
		}
	}

	return 0;
}