#include <iostream>
#include <string>

using namespace std;

int main(void)
{
	string s1;  // 使用无参构造 string() 构造一个长度为 0 的字符串
	string s2("HelloWorld");  // 使用 string(const char *s) 构造，初始化一个字符串
	string s3(s2);  // 使用 string(const char &str) 拷贝函数，拷贝 s2 来初始化 s3
	string s4(s3, 5);  // 使用 string(const string &str, size_t pos) 构造函数，从 s3 的索引 5 位置开始拷贝子串（即"World"）

	cout << "s1 = " << s1 << endl;
	cout << "s2 = " << s2 << endl;
	cout << "s3 = " << s3 << endl;
	cout << "s4 = " << s4 << endl;

	// ---------- 求字符串长度 ----------
	// size() 和 length() 是相同的，都是返回字符串的长度（不含 \0）
	cout << "s1.size = " << s1.size() << endl;  // s1.size = 0
	cout << "s2.size = " << s2.size() << endl;  // s2.size = 10
	cout << "s3.size = " << s3.size() << endl;  // s3.size = 10
	cout << "s4.size = " << s4.size() << endl;  // s4.size = 5

	cout << "s1.length = " << s1.length() << endl;  // s1.length = 0
	cout << "s2.length = " << s2.length() << endl;  // s2.length = 10
	cout << "s3.length = " << s3.length() << endl;  // s3.length = 10
	cout << "s4.length = " << s4.length() << endl;  // s4.length = 5

	// ---------- 字符串取值 ----------
	cout << "s2[2] = " << s2[2] << endl;        // s2[2] = l
	cout << "s2.at(2) = " << s2.at(2) << endl;  // s2.at(2) = l

	// ---------- 追加字符串 ----------
	s1.append("你好呀");
	cout << "s1 = " << s1 << endl;  // s1 = 你好呀
	s1 += "，帅哥！";
	cout << "s1 = " << s1 << endl;  // s1 = 你好呀，帅哥！

	// ---------- 插入字符串 ----------
	s2.insert(5, "_xinming_");
	cout << "s2 = " << s2 << endl;  // s2 = Hello_xinming_World

	// ---------- 删除字符串 ----------
	s2.erase(5, 9);
	cout << "s2 = " << s2 << endl;  // s2 = HelloWorld

	return 0;
	
}