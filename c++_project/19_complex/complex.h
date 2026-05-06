#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>

using namespace std;

// 复数类
class Complex
{
public:
	Complex(float real = 0.0, float imaginary = 0.0);  // 全缺省构造函数
	
	// 加法运算符重载，重载 operator+() 函数（类内部函数，能使用 this 指针）
	Complex operator+(const Complex &c);
	
	// 关系运算符重载，重载 operator==() 函数
	bool operator==(const Complex &c) const;

	// 友元函数，减法运算符重载，重载 operator-() 函数（本质是类外部函数，不能使用 this 指针，所以要两个参数）
	friend Complex operator-(const Complex &c1, const Complex &c2);
	
	// 友元函数，输出运算符重载
	friend ostream &operator<<(ostream &out, const Complex &c);

	// 友元函数，输入运算符重载
	friend istream &operator>>(istream &in, Complex &c);
	
	void show() const;

private:
	float m_real;       // 实部
	float m_imaginary;  // 虚部
};

#endif