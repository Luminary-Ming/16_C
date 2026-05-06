#include "mystring.h"
#include <cstring>

// 构造函数：默认空字符串
Mystring::Mystring(const char *str)
{
	if (str == nullptr)  // 如果是空指针，默认空字符串
		str = "";

	m_length = std::strlen(str);
	m_str = new char[m_length + 1];  // 开辟堆区空间存放字符串数据，+1 是给 \0 的空间
	std::strcpy(m_str, str);
}

// 析构函数：释放动态内存
Mystring::~Mystring()
{
	delete[] m_str;
}

// 深拷贝构造函数
Mystring::Mystring(const Mystring &ms)
{
	m_length = ms.m_length;
	m_str = new char[m_length + 1];
	std::strcpy(m_str, ms.m_str);
}

// 赋值运算符重载（深拷贝）
Mystring &Mystring::operator=(const Mystring &ms)
{
	if (this != &ms)  // 避免自赋值
	{
		delete[] m_str;  // 释放旧内存
		m_length = ms.m_length;
		m_str = new char[m_length + 1];
		std::strcpy(m_str, ms.m_str);
	}
	return *this;
}

// + 运算符重载：连接两个字符串
Mystring Mystring::operator+(const Mystring &ms) const
{
	size_t new_len = m_length + ms.m_length;  // 连接后的总长度
	char *temp = new char[new_len + 1];
	std::strcpy(temp, m_str);     // 复制 this->m_str
	std::strcat(temp, ms.m_str);  // 拼接 ms.m_str
	Mystring ret(temp);  // 调用构造函数实例化对象 ret
	delete[] temp;

	return ret;  // 返回对象 ret
}

// += 运算符重载：拼接字符串
Mystring &Mystring::operator+=(const Mystring &ms)
{
	size_t new_len = m_length + ms.m_length;  // 拼接后的总长度
	char *temp = new char[new_len + 1];
	std::strcpy(temp, m_str);     // 复制 this->m_str
	std::strcat(temp, ms.m_str);  // 拼接 ms.m_str
	delete[] m_str;  // 释放旧内存
	m_str = temp;    // 赋值新字符串（新指向） 
	m_length = new_len;  // 赋值新长度
	return *this;  // 返回当前对象
}

// == 运算符重载：比较相等
bool Mystring::operator==(const Mystring &ms) const
{
	return std::strcmp(m_str, ms.m_str) == 0;
}

// != 运算符重载：比较不等
bool Mystring::operator!=(const Mystring &ms) const
{
	return !(*this == ms);  // 直接复用 == 运算符重载的函数
}

// > 运算符重载：按字典顺序大于
bool Mystring::operator>(const Mystring &ms) const
{
	return std::strcmp(m_str, ms.m_str) > 0;
}

// < 运算符重载：按字典顺序小于
bool Mystring::operator<(const Mystring &ms) const
{
	return std::strcmp(m_str, ms.m_str) < 0;
}

// [] 运算符重载：返回值字符引用，可修改
char &Mystring::operator[](int index)
{
	return m_str[index];
}

// [] 运算符重载：返回值字符引用，只读（常函数版本）
const char &Mystring::operator[](int index) const
{
	return m_str[index];
}

// >> 输入运算符重载：从流中读取输入数据（以空白分隔）到 Mystring
istream &operator>>(istream &in, Mystring &ms)
{
	char buf[4096];  // 缓冲区
	in >> buf;  // 输入读取数据
	ms = Mystring(buf);  // 将读取的字符串赋值给 ms（利用以实现的 赋值运算符重载）
	return in;
}

// << 输出运算符重载：将 Mystring 内容输出到流
ostream &operator<<(ostream &out, const Mystring &ms)
{
	out << ms.m_str;
	return out;
}