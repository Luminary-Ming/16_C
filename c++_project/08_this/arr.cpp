#include "arr.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

/*
	this 是一个指向当前对象本身的指针
	它是一个隐藏的指针，在每个非静态成员函数中都可以使用
	指向调用该成员函数的那个对象
*/

using namespace std;  // 打开 std 命名空间

// 构造函数 (带默认参数)
VLA::VLA(int len)
{
	cout << "构造函数被调用了 !" << endl;  // 打印提示语句
	
	if (len > 0)
	{
		this->m_a = new int[len];  // 开辟 len 个 int 类型成员的数组空间
		this->m_len = len;         // 存储数组长度
	}
	else  // len < 0, 参数不合法
	{
		this->m_a = NULL;  // m_a 指向 NULL
		this->m_len = 0;
		cout << "参数 len 不合法 !" << endl;  // 打印提示语句
	}
}

// 拷贝构造函数 (深拷贝)
VLA::VLA(const VLA &v)
{
	cout << "拷贝构造函数被调用了 !" << endl;  // 打印提示语句
	
	this->m_len = v.m_len;

	this->m_a = new int[this->m_len];  // 给新对象开辟新空间

	memcpy(this->m_a, v.m_a, this->m_len * sizeof(int));  // 深拷贝
}

// 析构函数 (析构函数不能重载)
VLA::~VLA()
{
	cout << "析构函数被调用了 !" << endl;  // 打印提示语句

	delete[] m_a;  // 释放开辟的空间
	this->m_a = NULL;    // 防止野指针
	this->m_len = 0;     // 可变长数组长度清零
}

// 返回下标为 i 的元素
int VLA::at(int i)
{
	if (i >= this->m_len || i < 0 || this->m_a == NULL)  // 参数不合法
		abort();  // C++ 异常处理

	return this->m_a[i];  // 返回下标为 i 的元素
}

// 给下标为 i 的元素赋 value 值
int VLA::setValue(int i, int value)
{
	if (i >= this->m_len || i < 0 || this->m_a == NULL)  // 参数不合法
		abort();  // 异常终止

	this->m_a[i] = value;  // 赋值

	return 0;
}

// 遍历可变长数组
void VLA::show()
{
	if (this->m_a == NULL)
		abort();  // 异常终止

	for (int i = 0; i < this->m_len; i++)
		cout << "arr[" << i << "] = " << this->m_a[i] << endl;
}


// 打印 this 指针
void VLA::printThis()
{
	cout << "this = " << this << endl;
}

