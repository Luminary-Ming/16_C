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

// 在数组末尾插入一个新元素
int VLA::insert(int value)
{
	if (this->m_a == NULL)
		return -1;

	// 开辟新数组, 长度 = 原长度 + 1
	int *new_a = new int[this->m_len + 1];  

	// 将原数组的所有元素复制到新数组的前面部分
	memcpy(new_a, this->m_a, this->m_len * sizeof(int));

	// 将新值放到数组的最后位置
	new_a[this->m_len] = value;

	delete[] this->m_a;  // 释放原数组占用的堆内存
	this->m_a = new_a;   // 让成员数组指针指向新数组
	this->m_len++;       // 更新数组长度

	return this->m_len - 1;  // 返回新插入元素的位置
}

// 删除指定位置的元素
int VLA::del(int i)
{
	if (i < 0 || i >= this->m_len)
		return -1;

	// 开辟新数组, 长度 = 原长度 - 1
	int *new_a = new int[this->m_len - 1];

	// 复制元素, 跳过要删除的那个元素
	for (int j = 0, k = 0; j < this->m_len; j++)
	{
		// 如果当前元素不是要删除的元素, 就复制到新数组
		if (j != i)
			new_a[k++] = m_a[j];  // 先赋值, 然后 k 自增 1
	}

	delete[] this->m_a;  // 释放原数组占用的堆内存
	this->m_a = new_a;   // 让成员指针指向新数组
	this->m_len--;       // 更新数组长度 (减少 1)

	return 0;
}

// 更新指定位置的元素值
int VLA::update(int i, int value)
{
	if (this->m_a == NULL || i < 0 || i >= this->m_len)
		return -1;

	this->m_a[i] = value;

	return 0;
}

// 查询指定位置的元素值
int VLA::select(int i)
{
	if (this->m_a == NULL || i < 0 || i >= this->m_len)
		return -1;

	return this->m_a[i];
} 

