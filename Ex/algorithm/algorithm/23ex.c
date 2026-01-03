/*
	若 ax^2 + bx + c = 0 有实根，则求之

	有实根 b^2 + 4ac >= 0
*/

#include <stdio.h>
#include <math.h>

int main(void)
{

	double a, b, c;  // 系数
	double x1, x2;  // 两个实数根
	double delta;  // Δ ( 德尔塔 )

	printf("求 ax^2 + bx + c = 0 的实数根");

	printf("输入三个系数 a b c\n");
	scanf("%lf %lf %lf", &a, &b, &c);

	if (a == 0) {
		printf("a = 0, 转变成求 bx + c = 0 的实数根\n");

		if (b == 0) {
			if (c == 0) {
				printf("a = 0, b = 0, 该方程为恒等式!\n");
			}
			else {
				printf("%f = 0 该方程是矛盾方程（无解）!\n", c);
			}
		}
		else {
			x1 = -c / b;
			x2 = x1;
			printf("该方程为有一个实数根, 根为 %f\n", x1);
		}
	}
	else {
		delta = b * b - 4 * a * c;
		if (delta < 0) {
			printf(" Δ < 0 该方程没有实数根!\n");
		}
		else if (delta == 0) {
			x1 = -b / (2 * a);
			x2 = x1;
			printf(" Δ = 0 该方程有两个相同的实数根!\n");
			printf("x1 = x2 = %f\n", x1);
		}
		else{
			x1 = (-b + sqrt(delta)) / (2 * a);
			x2 = (-b - sqrt(delta)) / (2 * a);
			printf(" Δ > 0 该方程有两个不同的实数根!\n");
			printf("x1 = %f, x2 = %f\n", x1, x2);
		}
	}
}