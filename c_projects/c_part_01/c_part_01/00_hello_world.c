/*
	#include 是一个预处理命令, 用来引入头文件

	stdio.h 是一个头文件 ( 标准输入输出头文件 )

	当编辑器遇到 printf() 函数时, 如果没有找到 stdio.h 头文件, 就会报编译错误
*/
#include <stdio.h>

// 代码从 main() 函数开始执行
int main(void)
{
	// printf() 函数在 "stdio.h" 头文件中声明
	printf("Hello World!\n");

	// 用于表示退出程序
	return 0;
	
}
/*
	return 常见的含义
	1. 程序/函数执行状态（退出码）
	return 0;    程序 / 函数 执行成功, 没有错误

	return 非0;  程序 / 函数 执行失败, 有错误
		不同的数字可以代表不同的错误类型。
		· 1 可能表示 “ 一般性错误 ”
		· 2 可能表示 “ 语法错误 ”
		等等。具体含义由操作系统或程序自己约定

	2. 表示逻辑真/假（布尔值）
	在C语言等没有内置布尔类型的语言中，常用整数来表示逻辑值。
	return 1; -> 通常表示 "真"
	return 0; -> 通常表示 "假"

	例如 : 检查是否为偶数的函数
	int isEven(int number) {
		if (number % 2 == 0) {
			return 1; // 是真（是偶数）
		} else {
			return 0; // 是假（不是偶数）
		}
	}

	3. 表示具体的计算结果, 返回值

	int add(int a, int b) {
		return a + b; // 返回两个数的和
	}
*/