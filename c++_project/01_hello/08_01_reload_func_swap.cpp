#include <iostream>

/*
	实现 swap() 即可交换整型数也可以交换浮点数
*/

// swap 重载
inline void swap(int *a, int *b); // 交换两个 int   (指针实现)
void swap(float &a, float &b);    // 交换两个 float (引用实现)

int main(void)
{
	int a = 0, b = 0;
	float c = 0.0f, d = 0.0f;

	// 用户输入
	std::cout << "请输入两个整数: ";
	std::cin >> a >> b;
	std::cout << "请输入两个小数: ";
	std::cin >> c >> d;

	// 交换两个整数
	std::cout << "\n交换前: a = " << a << ", b = " << b << std::endl;
	swap(&a, &b);
	std::cout << "交换后: a = " << a << ", b = " << b << std::endl;

	// 交换两个小数
	std::cout << "交换前: c = " << c << ", d = " << d << std::endl;
	swap(c, d);
	std::cout << "交换后: c = " << c << ", d = " << d << std::endl;

	return 0;
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void swap(float &a, float &b)
{
	float temp = b;
	b = a;
	a = temp;
}

/*
	请输入两个整数: 22 33
	请输入两个小数: 22.22 33.33

	交换前: a = 22, b = 33
	交换后: a = 33, b = 22
	交换前: c = 22.22, d = 33.33
	交换后: c = 33.33, d = 22.22
*/