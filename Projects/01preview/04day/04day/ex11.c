/*
	打印 0~100 之间的质数
	质数: 只能被 1 和本身整除

	思路:
		0 和 1 不是质数, 2 是最小的质数, 偶数不可能是质数, 只能是奇数
*/
#include <stdio.h>
#include <stdbool.h>


// 判断一个数是否为质数
bool is_prime(int n) {
	// 0 和 1 不是质数
	if (n <= 1) {
		return false;
	}

	// 2 是质数
	if (n == 2) {
		return true;
	}

	// 偶数不是质数
	if (n % 2 == 0) {
		return false;
	}

	// 方法一 : 判断 n 是否能被 2 ~ n-1 整除, 如果能就不是质数
	/*for (int i = 2; i < n; i++) {
		if (n % i == 0) {
			return false;
		}
	}*/

	/*
		方法二: 检查奇数 n 的奇数因子
		只有 奇数 * 奇数 = 奇数
		如果奇数 n 有因子 a 和 b, 且 a*b = n
		其中一个因子必然 ≤ √n
		另一个因子必然 ≥ √n
		所以只需要检查到 √n 即可 ( 只要有一个奇数因子就说明能整除, 就不是质数 )
	*/ 
	for (int i = 3; i * i <= n; i+= 2) {
		if (n % i == 0) {
			return false;
		}
	}

	return true;
}


int main()
{
	for (int n = 0; n <= 100; n++) {
		if (is_prime(n)) {
			printf("%d\n", n);
		}
	}
	return 0;
}