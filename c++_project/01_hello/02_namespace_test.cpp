#include <iostream>

using namespace std;  // 展开 std 命名空间

// 自定义命名空间 myTest, 用于封装我们自己的标识符, 避免与标准库冲突
// 这里定义一个与标准库同名的 rand 变量, 由于位于不同的命名空间, 所以不会产生冲突
namespace myTest
{
	int rand;  // 定义全局变量
}  // 注意: 命名空间定义结束时不需要加分号 (尽管加上也不报错, 但遵循规范不加)

int main()
{
	// 使用作用域解析符 :: 访问 myTest 命名空间中的 rand 变量
	myTest::rand = 2233;  // 给全局变量赋值

	// 由于使用了 using namespace std; 可以直接写 cout 和 endl
	cout << "rand = " << myTest::rand << endl;  // 打印全局变量的值

	return 0;
}