#include <iostream>
#include "mystring.h"

int main(void)
{
	using std::cout;
	using std::endl;

	Mystring str1;           // 使用空参构造实例化空字符串 ""
	Mystring str2("你好呀");  // 使用有参构造实例化 str2 字符串
	Mystring str3(str2);     // 使用拷贝构造函数初始化 str3 字符串

	cout << "str1 = " << str1 << endl;  // str1 =
	cout << "str2 = " << str2 << endl;  // str2 = 你好呀
	cout << "str3 = " << str3 << endl;  // str3 = 你好呀

	str2 += str3;  // 拼接字符串
	cout << "str2 = " << str2 << endl;  // str2 = 你好呀你好呀

	Mystring str4("帅哥");
	str1 = str3 + str4;  // 连接字符串
	cout << "str1 = " << str1 << endl;  // str1 = 你好呀帅哥

	// 关系运算符测试
	Mystring str5("abc");
	Mystring str6("abc");
	Mystring str7("def");
	cout << "str5 == str6 ? " << (str5 == str6) << endl;  // str5 == str6 ? 1
	cout << "str5 == str7 ? " << (str5 == str7) << endl;  // str5 == str7 ? 0
	cout << "str5 != str6 ? " << (str5 != str6) << endl;  // str5 != str6 ? 0
	cout << "str5 != str7 ? " << (str5 != str7) << endl;  // str5 != str7 ? 1
	cout << "str5 > str7 ? " << (str5 > str7) << endl;    // str5 != str7 ? 1
	cout << "str5 < str7 ? " << (str5 < str7) << endl;    // str5 != str7 ? 1
	
	// 下标运算符测试
	str5[1] = 'h';
	cout << "str5 = " << str5[1] << endl;  // str5 = h
	cout << "str5 = " << str5 << endl;     // str5 = ahc

	const Mystring str8("jkl");  // 常对象，只读
	cout << "str8 = " << str8[1] << endl;  // str8 = k
	cout << "str8 = " << str8 << endl;     // str8 = jkl

	return 0;
}