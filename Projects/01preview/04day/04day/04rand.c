#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	int a = 0;  // 循环变量
	int i = 0;  // 存储获取到的随机数

	//srand(time(NULL));  // 把时间戳作为随机种子
	srand(time(NULL) + getpid());  // 把当前调用进程的 ID 号 + 时间作为随机种子使用

	for (a = 0; a < 5; a++)
	{
		//i = rand() % 1000;
		//i = rand() % 90 + 10;
		i = rand() % 100 - 50;
		printf("i = %d\n", i);
	}

	return 0;
}

/*
	rand() 函数生成的是一系列可预测的数值序列

	给定相同的种子，rand() 会产生完全相同的随机数序列
	这保证了程序的可重复性（对调试有用）

	相同种子产生相同序列
	srand(123);  // 种子固定
	rand(); rand(); rand();  // 总是相同的三个数


	如果不调用 srand()：
	系统会默认使用种子 1（srand(1)）
	每次程序运行都会得到相同的随机数序列

	没有 srand()，每次运行输出相同
	for (int i = 0; i < 3; i++) {
		printf("%d ", rand());  // 总是 41, 18467, 6334...
	}

	通过使用变化的种子，让每次运行产生不同的序列：
	常用种子来源：
	srand(time(NULL));             // 使用当前时间（秒级）
	srand(getpid());               // 使用进程 ID
	srand(time(NULL) + getpid());  // 时间+进程ID，更随机


*/