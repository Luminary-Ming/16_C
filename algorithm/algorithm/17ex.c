/*
	从终端输入三个正实数，判断这三个数能否构成直角三角形。
*/
#include <stdio.h>

int main()
{
	int a, b, c;
	scanf("%d %d %d", &a, &b, &c);

	if (a <= 0 || b <= 0 || c <= 0) {
		printf("请输入大于0的数字");
	}


	if (a * a + b * b == c * c || a * a + c * c == b * b || b * b + c * c == a * a) {
		printf("能构成直角三角形");
	}
	else {
		printf("不能构成直角三角形");
	}
}