#include <stdio.h>
/*
	从键盘输入 20 个数据到数组中，统计其中正数的个数，并计算这些正数之和。
*/

#define SIZE 20  // 定义数组大小

int main()
{
	double nums[SIZE];  // 定义一个大小为20的数组
	int positive_count = 0;  // 正数个数计数器
	double positive_sum = 0.0;  // 正数之和
	int i = 0;  // 循环变量

	printf("请输入20个数字（用空格或回车分隔）:\n");

	// 用户输入数字存入数组
	for (i = 0; i < SIZE; i++) {
		scanf("%lf", &nums[i]);  // %lf 用于读取 scanf() 函数中的 double 类型
	}

	// 遍历数组，统计正数个数和计算正数之和
	for (i = 0; i < SIZE; i++) {
		if (nums[i] > 0) {
			positive_count++;
			positive_sum += nums[i];
		}
	}

	printf("正数的个数: %d\n", positive_count);
	printf("正数之和: %.2f\n", positive_sum);  // %.2f 四舍五入保留两位小数输出

	return 0;
}

/*
	
	数据类型	   scanf()		printf()	说明

	float		   %f			%f			scanf() 中 %f 专用于 float

	double		   %lf			%f			scanf() 中 %lf 专用于 double

	long double	   %Lf			%Lf			大写 L

*/