#include <iostream>
#include "complex.h"

using namespace std;

int main(void)
{
	Complex c1(1.2, 3.4);  // 实例化 c1 对象
	Complex c2(5.6, 7.8);  // 实例化 c2 对象
	Complex c3;  // 实例化 c3 对象
	Complex c4;  // 实例化 c4 对象

	c3 = c1 + c2;
	c3.show();  // 打印：z = 6.8 + 11.2i

	c4 = c2 - c1;
	c4.show();  // 打印：z = 4.4 + 4.4i

	Complex c5(1.2, 3.4);  // 实例化 c5 对象
	cout << "c1 == c2 ? " << (c1 == c2) << endl;  // 打印：c1 == c2 ? 0
	cout << "c1 == c5 ? " << (c1 == c5) << endl;  // 打印：c1 == c5 ? 1

	cout << c1 << endl;  // 打印：z = 1.2 + 3.4i

	cin >> c4;           // 输入：22.22 33.33
	cout << c4 << endl;  // 打印：z = 22.22 + 33.33i

	return 0;
}