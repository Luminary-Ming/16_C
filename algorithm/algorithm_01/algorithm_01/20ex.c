/*
	输入N, 随机产生N个大写字母输出，然后统计其中共有多少个元音字符。

	元音字符 : A E I O U
	A 65 ~ Z 90
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
	int N;
	scanf("%d", &N);

	int count = 0;

	srand(getpid() + time(NULL));  // 把当前调用的进程 ID + 时间作为随机种子

	int letter;

	printf("\n生成的大写字母有: \n");

	for (int i = 1; i <= N; i++) {
		letter = rand() % 26 + 65;

		// 输出字母，每行10个
		printf("%c ", letter);
		if (i % 10 == 0) {
			printf("\n");
		}

		// 统计元音
		if (letter == 'A' || letter == 'E' || letter == 'I' || letter == 'O' || letter == 'U') {
			count++;
		}
	}

	printf("\n共有 %d 个元音字符", count);
	return 0;
}