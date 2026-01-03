/*
	输出 n 行由 '#' 组成的矩形，每行 '#' 的个数为 m 个，
	其中n、m由终端（键盘）输入。
	要求输出 m 个 '#' 功能由函数 satr(m) 函数实现。
*/

#include <stdio.h>

// 每行输出 m 个 '#'
void satr(m) {
	for (int j = 1; j <= m; j++) {
		printf("#");
	}
}

int main(void) {

	int n = 0, m = 0;

	scanf("%d %d", &n, &m);

	// 输出 n 行由 '#' 组成的矩形
	for (int i = 1; i <= n; i++) {
		satr(m);
		printf("\n");
	}

	return 0;
}