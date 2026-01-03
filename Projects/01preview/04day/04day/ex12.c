/*
	猜数游戏:
		输入一个 n ,随机产生一个 n 以内的数字去猜
		如果大了,打印猜大了
		如果小了,打印猜小了
		如果正确,打印猜对了
		循环知道猜对了结束, 每次打印带上猜的次数
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
	int n;  // 要猜的数的范围
	int m;  // 乱猜的数
	int count = 0;  // 猜的次数
	printf("要猜的数的范围\n");
	scanf("%d", &n);


	srand(getpid());  // 把当前调用进程的 ID 号作为随机种子使用

	// 随机产生 n 以内的数字
	int i = rand() % (n + 1);
	printf("要猜的数是: %d\n", i);

	printf("开始猜吧:\n");

	do {
		count++;

		printf("第 %d 次, 请输入你猜的数字 : ", count);
		scanf("%d", &m);

		if (m > i) {
			printf("猜大了!\n");
		}
		else if (m < i) {
			printf("猜小了!\n");
		}
		else {
			printf("猜对了!\n");
			break;
		}
	} while (1);  // 无限循环直到猜对

	return 0;
}