#include "complex.h"
#include <iostream>

Complex::Complex(float real, float imaginary) :m_real(real), m_imaginary(imaginary) 
{
}

Complex Complex::operator+(const Complex &c)
{
	Complex ret;  // 返回的对象

	// 返回对象的实部 = 当前对象的实部 + 参数对象的实部
	ret.m_real = this->m_real + c.m_real;
	// 返回对象的虚部 = 当前对象的虚部 + 参数对象的虚部
	ret.m_imaginary = this->m_imaginary + c.m_imaginary;

	return ret;  // 返回对象
}

bool Complex::operator==(const Complex &c) const
{
	return this->m_real == c.m_real && this->m_imaginary == c.m_imaginary;
}

Complex operator-(const Complex &c1, const Complex &c2)
{
	Complex ret;  // 返回的对象
	// 返回对象的实部 = 参数 c1 对象的实部 + 参数 c2 对象的实部
	ret.m_real = c1.m_real - c2.m_real;
	// 返回对象的虚部 = 参数 c1 对象的虚部 + 参数 c2 对象的虚部
	ret.m_imaginary = c1.m_imaginary - c2.m_imaginary;

	return ret;  // 返回对象
}

ostream &operator<<(ostream &out, const Complex &c)
{
	out << "z = " << c.m_real << " + " << c.m_imaginary << "i";
	return out;
}

istream &operator>>(istream &in, Complex &c)
{
	in >> c.m_real >> c.m_imaginary;
	return in;
}

void Complex::show() const
{
	// z = a + bi
	std::cout << "z = " << this->m_real << " + " << this->m_imaginary << "i" << std::endl;
}
