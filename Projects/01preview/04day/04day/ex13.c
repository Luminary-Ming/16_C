/*
	a + b - 9 = 4
	+   -   -
	c - d * e = 4
	/   *   -
	f + g - h = 4
	=   =   =
	4   4   4
	求 a b c d e f g h 的值( 0~12 范围内), 其中除法是整除没有余数

	a + b - 9 = 4
	c - d * e = 4
	f + g - h = 4
	a + c / f = 4
	b - d * g = 4
	9 - e - h = 4
*/
#include <stdio.h>
#include <stdbool.h>

// 检查值是否在 0~12 范围内 
bool check_size(int n) {
	if (n >=0 && n<=12) return true;
	return false;
}


int main()
{
	// a + b - 9 = 4
	for (int a = 0; a <= 12; a++) {
		int b = 4 + 9 - a;
		if (!check_size(b)) continue;

		// 9 - e - h = 4
		for (int e = 0; e <= 12; e++) {
			int h = 9 - e - 4;
			if (!check_size(h)) continue;

			// a + c / f = 4
			for (int f = 1; f <= 12; f++) {
				int c = (4 - a) * f;
				if (!check_size(c)) continue;

				// b - d * g = 4 , g = (b - 4) / d, d 不能是 0 
				for (int d = 1; d <= 12; d++) {
					if ((b - 4) % d != 0) continue;  // 必须是整除
					int g = (b - 4) / d;
					if (!check_size(g)) continue;

					// c - d * e == 4
					// f + g - h == 4
					if ((c - d * e == 4) && (f + g - h == 4)) {
							printf("a=%d b=%d c=%d d=%d e=%d f=%d g=%d h=%d\n",
								a, b, c, d, e, f, g, h);  // a=2 b=11 c=4 d=1 e=0 f=2 g=7 h=5
							return 0;
					}
				}
			}
		}
	}

	printf("没有解\n");

	return 0;
}