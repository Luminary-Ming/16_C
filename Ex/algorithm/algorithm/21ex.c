/*
	找出三位自然数中的所有素数，要求判断x素数用自定义函数data(x)实现

	素数就是质数
*/

#include <stdio.h>

int is_primeNumber(int x) {
	if (x <= 1) return 0;  // 0 和 1 不是素数

	if (x == 2) return 1;  // 2是素数

	if (x % 2 == 0) return 0;  // 偶数不是素数

	// 如果 x 在 2 到 x-1 之间能被整除说明 x 不是素数
	//for (int i = 2; i < x; i++) {
	//	if (x % i == 0) {
	//		return 0; // 不是素数
	//	}
	//} 

	/*
		检查奇数 n 的奇数因子
		只有 奇数 * 奇数 = 奇数
		如果奇数 n 有因子 a 和 b, 且 a*b = n
		其中一个因子必然 ≤ √n
		另一个因子必然 ≥ √n
		所以只需要检查到 √n 即可 ( 只要有一个奇数因子就说明能整除, 就不是质数 )
	*/
	for (int i = 3; i * i <= x; i += 2) {
		if (x % i == 0) {
			return 0;
		}
	}
	
	return 1;  // 是素数
}

int main()
{
	int count = 0;

	printf("三位自然数中的素数有:\n");

	for (int i = 100; i <= 999; i++) {
		if (is_primeNumber(i)) {
			count++;
			printf("%d\t", i);

			// 每十个换行
			if (count % 10 == 0) {
				printf("\n");
			}
		}
	}
}