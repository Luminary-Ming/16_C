#ifndef MYSTRING_H
#define MYSTRING_H

#include <cstdlib>
#include <iostream>

using namespace std;

/*
	定义一个 Mystring 类，实现运算符重载：+、+=、==、!=、>、<、[]
*/
class Mystring
{
public:
	// 构造函数
	Mystring(const char *str = "");

	// 析构函数
	~Mystring();

	// 因为成员变量中有动态内存成员 char *m_str，所以要声明 深拷贝构造函数 和 赋值运算符重载
	Mystring(const Mystring &str);            // 拷贝构造函数（深拷贝）
	Mystring &operator=(const Mystring &ms);  // 赋值运算符重载

	// 加法运算符重载，加法不修改左操作数，声明为 const 成员函数
	Mystring operator+(const Mystring &ms) const;

	// += 运算符重载
	Mystring &operator+=(const Mystring &ms);

	// 关系运算符重载，==、!=、>、< 不修改对象，声明为 const 成员函数
	bool operator==(const Mystring &ms) const;
	bool operator!=(const Mystring &ms) const;
	bool operator>(const Mystring &ms) const;
	bool operator<(const Mystring &ms) const;

	// 下标运算符重载
	char &operator[](int index);
	const char &operator[](int index) const;  // 常函数，适用于常对象

	// 友元函数，>> << 输入输出运算符重载
	friend istream &operator>>(istream &in, Mystring &ms);
	friend ostream &operator<<(ostream &out, const Mystring &ms);

private:
	char *m_str;
	size_t m_length;
};

#endif