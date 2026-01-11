#include <stdio.h>
/*
	要将5张100元的大钞票，换成等值的50元，20元，10元，5元的小钞票，
	每种面值至少1张，编程求有多少种换法, 总共最少能换几张, 最多能换几张 
*/
int main()
{
	int count = 0;  // 换法总数
	int min_sheets = 1000; // 最少张数（初始设大值）
	int max_sheets = 0; // 最多张数

	/*
		每种面值至少 1 张时 :
		50 最多 9 张 : (500 - 20 - 10 - 5) = 465 / 50 = 9.3
		20 最多 21 张 : (500 - 50 - 10 - 5) = 435 / 20 = 21.75
		10 最多 42 张 : (500 - 50 - 20 - 5) = 425 / 10 = 42.5
	*/
	for (int m1 = 1; m1 <= 9; m1++) {
		for (int m2 = 1; m2 <= 21; m2++) {
			for (int m3 = 1; m3 <= 42; m3++) {
				int balance = 500 - 50 * m1 - 20 * m2 - 10 * m3;  // 余额
				// 保证换完 50块 20块 10块 的还有余额, 并且能换整数个 5 块的
				if (balance > 0 && balance % 5 == 0) {
					int m4 = balance / 5;  // 5块的张数

					// 最少能换一张 5 块的
					if (m4 >= 1) {
						count++;  // 换法总数 + 1
						int total_sheets = m1 + m2 + m3 + m4; // 总张数

						// 最少换几张
						if (total_sheets < min_sheets) {
							min_sheets = total_sheets;
						}

						// 最多换几张
						if (total_sheets > max_sheets) {
							max_sheets = total_sheets;
						}
					}
				}
			}
		}
	}

	printf("最多有 %d 种换法\n", count);  // 1564 种换法
	printf("最多能换 %d 张\n", max_sheets);  // 87 张 1*50 + 1*20 + 1*10 + 84*5 = 500
	printf("最少能换 %d 张\n", min_sheets);  // 14 张 9*50 + 1*20 + 2*10 + 2*5 = 500

	return 0;
}