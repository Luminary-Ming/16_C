/*
	打印 100 以内所有的质数 ( 素数 )

	质数 : 只能被 1 和本身整除的数字叫质数

	0, 1 不是质数, 最小的质数是 2
*/
#include <stdio.h>

/*
	real    0m2.656s  从开始到结束的总时间
	user    0m2.624s  程序在用户态运行的时间
	sys     0m0.027s  程序在内核态运行的时间
*/
int is_prime_number1(int i) {

	for (int j = 2; j <= i - 1; j++) {
		if (i % j == 0) {
			return 0;  // 不是质数
		}
	}

	return 1;  // 是质数
}

/*
	real    0m1.341s
	user    0m1.321s
	sys     0m0.019s
*/
int is_prime_number2(int i) {

	for (int j = 2; j <= i / 2; j++) {
		if (i % j == 0) {
			return 0;  // 不是质数
		}
	}

	return 1;  // 是质数
}

/*
	real    0m0.004s
	user    0m0.000s
	sys     0m0.004s
*/
int is_prime_number3(int i) {
	/*
		拿 24 举例, 它的因子中间值是 √24

		1 2 3 4  √24  6 8 12 24

		所以只需要判断 √i 以内的数有没有 i 的因子就行了, 如果有说明不是质数
	*/
	for (int j = 2; j * j <= i; j++) {
		if (i % j == 0) {
			return 0;  // 不是质数
		}
	}

	return 1;  // 是质数
}


int main(void)
{
	int count = 0;  // 计数

	// 遍历 2 ~ 100
	for (int i = 2; i <= 100; i++) {
		if (is_prime_number3(i)) {
			count++;
			printf("%3d", i);

			// 每十个换行
			if (count % 10 == 0) {
				printf("\n");
			}
		}
	}

	return 0;
}